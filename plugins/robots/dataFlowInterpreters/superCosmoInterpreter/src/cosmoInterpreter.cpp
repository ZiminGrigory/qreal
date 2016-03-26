#include "cosmoInterpreter.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>
#include <QtCore/QQueue>
#include <QtCore/QPair>

#include <qrtext/languageToolboxInterface.h>

#include <utils/timelineInterface.h>
#include <kitBase/robotModel/robotModelInterface.h>


using namespace qReal;
using namespace interpretation;
using namespace interpreterCore;
using namespace interpreterCore::interpreter;
using namespace interpreterCore::interpreter::dataFlowInterpretation;
using namespace dataFlowInterpretation;
using namespace kitBase::robotModel;

const int maxThreadsCount = 100;

CosmoInterpeter::CosmoInterpeter(const GraphicalModelAssistInterface &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, const qReal::ProjectManagementInterface &projectManager
		, BlocksFactoryManagerInterface &blocksFactoryManager
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &languageToolbox
		, QAction &connectToRobotAction
		)
	: mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mState(idle)
	, mRobotModelManager(robotModelManager)
	, mBlocksTable(new details::BlocksTable(blocksFactoryManager, robotModelManager))
	, mActionConnectToRobot(connectToRobotAction)
	, mSensorVariablesUpdater(robotModelManager, languageToolbox)
	, mAutoconfigurer(mGraphicalModelApi, *mBlocksTable, *mInterpretersInterface.errorReporter())
	, mLanguageToolbox(languageToolbox)
{
	// Other components may want to subscribe to allDevicesConfigured() signal because
	// it seems to be the only way to perform robot devices additional initialization.
	// We must let them work out before interpretation starts, so creating queued connection.
	connect(
			&mRobotModelManager
			, &kitBase::robotModel::RobotModelManagerInterface::allDevicesConfigured
			, this
			, &CosmoInterpeter::devicesConfiguredSlot
			, Qt::QueuedConnection
			);

	connect(
			&mRobotModelManager
			, &kitBase::robotModel::RobotModelManagerInterface::connected
			, this
			, &CosmoInterpeter::connectedSlot
			);

	connect(&projectManager, &qReal::ProjectManagementInterface::beforeOpen, this, &CosmoInterpeter::userStopRobot);

	connectDevicesConfigurationProvider(&mAutoconfigurer);
}

CosmoInterpeter::~CosmoInterpeter()
{
	qDeleteAll(mThreads);
	delete mBlocksTable;
}

void CosmoInterpeter::interpret()
{
	mInterpretersInterface.errorReporter()->clear();

	if (mRobotModelManager.model().connectionState() != RobotModelInterface::connectedState) {
		mInterpretersInterface.errorReporter()->addInformation(tr("No connection to robot"));
		return;
	}

	if (mState != idle) {
		mInterpretersInterface.errorReporter()->addInformation(tr("Interpreter is already running"));
		return;
	}

	mRobotModelManager.model().stopRobot();
	mBlocksTable->clear();
	mState = waitingForDevicesConfiguredToLaunch;

	if (!mAutoconfigurer.configure(mGraphicalModelApi.children(Id::rootId()), mRobotModelManager.model().robotId())) {
		mState = idle;
		return;
	}

	mLanguageToolbox.clear();

	/// @todo Temporarily loading initial configuration from a network of SensorConfigurationProviders.
	///       To be done more adequately.
	const QString modelName = mRobotModelManager.model().robotId();
	for (const PortInfo &port : mRobotModelManager.model().configurablePorts()) {
		const DeviceInfo deviceInfo = currentConfiguration(modelName, port);
		mRobotModelManager.model().configureDevice(port, deviceInfo);
	}

	mRobotModelManager.model().applyConfiguration();
}

void CosmoInterpeter::stopRobot(qReal::interpretation::StopReason reason)
{
	mSensorVariablesUpdater.suspend();
	mRobotModelManager.model().stopRobot();
	mState = idle;
	qDeleteAll(mThreads);
	mThreads.clear();
	mBlocksTable->setFailure();
	emit stopped(reason);
}

int CosmoInterpeter::timeElapsed() const
{
	return mState == interpreting
			? mRobotModelManager.model().timeline().timestamp() - mInterpretationStartedTimestamp
			: 0;
}

void CosmoInterpeter::connectedSlot(bool success, const QString &errorString)
{
	if (success) {
		if (mRobotModelManager.model().needsConnection()) {
			mInterpretersInterface.errorReporter()->addInformation(tr("Connected successfully"));
		}
	} else {
		if (errorString.isEmpty()) {
			mInterpretersInterface.errorReporter()->addError(tr("Can't connect to a robot."));
		} else {
			mInterpretersInterface.errorReporter()->addError(errorString);
		}
	}

	mActionConnectToRobot.setChecked(success);
}

void CosmoInterpeter::devicesConfiguredSlot()
{
	if (mRobotModelManager.model().connectionState() != RobotModelInterface::connectedState) {
		mInterpretersInterface.errorReporter()->addInformation(tr("No connection to robot"));
		mState = idle;
		return;
	}

	if (mState == waitingForDevicesConfiguredToLaunch) {
		mState = interpreting;
		mInterpretationStartedTimestamp = mRobotModelManager.model().timeline().timestamp();

		mSensorVariablesUpdater.run();

		initDiagramInterpetationProcess();
	}
}

IdList CosmoInterpeter::findStartChildrens(const Id &currentDiagramId)
{
	auto childrens = mGraphicalModelApi.children(currentDiagramId);
	IdList startElements;
	for (auto &id : childrens) {
		if (mGraphicalModelApi.graphicalRepoApi().incomingLinks(id).empty()) {
			startElements << id;
		}
	}

	return startElements;
}

/// @todo: make it with threads
void CosmoInterpeter::initDiagramInterpetationProcess()
{
	IdList startElements = findStartChildrens(mInterpretersInterface.activeDiagram());
	connectResolver.clear();
	handledElements.clear();

	prepareDiagramInterpretation(startElements, "main");
}

void CosmoInterpeter::prepareDiagramInterpretation(const IdList &startElements, const QString &threadId)
{
	QQueue<Id> nextBlocks;
	for (auto &id : startElements) {
		nextBlocks.enqueue(id);
	}

	while (!nextBlocks.empty()) {
		Id curId = nextBlocks.dequeue();
		auto curBlock = dynamic_cast<DataFlowRobotsBlock *>(mBlocksTable->block(curId));
		Id localOutgoingExplosion = mLogicalModelApi.logicalRepoApi().outgoingExplosion(curId);
		if (!localOutgoingExplosion.isNull()) {
			// explosion handle add and connect to start points
		}

		/// @todo: here check if block is IOblock then it connect to map for subprogram-diagramId connect to "resolver"

		IdList outgoingLinks = mGraphicalModelApi.graphicalRepoApi().outgoingLinks(curId);
		for (auto &linkId : outgoingLinks) {
			auto nextBlockId = mGraphicalModelApi.to(linkId);
			auto nextBlock = dynamic_cast<DataFlowRobotsBlock *>(mBlocksTable->block(nextBlockId));
			if (!handledElements.contains(nextBlockId)) {
				nextBlocks.enqueue(nextBlockId);
			}

			//	QMap<DataFlowRobotsBlock *, QMultiMap<int, QPair<DataFlowRobotsBlock *, int>>> connectResolver;
			connectResolver[curBlock].insert(
					qRound64(mGraphicalModelApi.fromPort(linkId))
					, QPair<DataFlowRobotsBlock *, int>(nextBlock, qRound64(mGraphicalModelApi.toPort(linkId)))
			);

			connect(
					curBlock
					, &DataFlowRobotsBlock::newDataInFlow
					, this
					, &CosmoInterpeter::handleDataInFlow
					, Qt::QueuedConnection
			);

		}

		handledElements.insert(curId);
	}

	auto * const initialThread = new CosmoThread(
			&mGraphicalModelApi
			, mInterpretersInterface
			, mBlocksTable
			, startElements
			, threadId
	);

	emit started();
	addThread(initialThread, threadId);
}

void CosmoInterpeter::handleDataInFlow(const QVariant &data, int port)
{
	QList<QPair<DataFlowRobotsBlock *, int>> localValues =
			connectResolver[static_cast<DataFlowRobotsBlock *>(sender())].values(port);

	for (QPair<DataFlowRobotsBlock *, int> &pair: localValues) {
		pair.first->handleDataFromFlow(data, pair.second);
	}
}

void CosmoInterpeter::threadStopped(qReal::interpretation::StopReason reason)
{
	auto * const thread = static_cast<CosmoThread *>(sender());

	mThreads.remove(thread->id());
	delete thread;

	if (mThreads.isEmpty()) {
		stopRobot(reason);
	}
}


/// @todo: rewrite this functions;
//void CosmoInterpeter::newThread(const Id &startBlockId, const QString &threadId)
//{
//	if (mThreads.contains(threadId)) {
//		reportError(tr("Cannot create new thread with already occupied id %1").arg(threadId));
//		stopRobot(qReal::interpretation::StopReason::error);
//		return;
//	}

//	qReal::interpretation::dataFlowInterpreter::CosmoThread * const thread =
//			new qReal::interpretation::dataFlowInterpreter::CosmoThread(&mGraphicalModelApi
//			, mInterpretersInterface, startingElementType, *mBlocksTable, startBlockId, threadId);

//	addThread(thread, threadId);
//}

void CosmoInterpeter::addThread(CosmoThread * const thread, const QString &threadId)
{
	if (mThreads.count() >= maxThreadsCount) {
		reportError(tr("Threads limit exceeded. Maximum threads count is %1").arg(maxThreadsCount));
		stopRobot(qReal::interpretation::StopReason::error);
	}

	mThreads[threadId] = thread;
	connect(
			thread
			, &CosmoThread::stopped
			, this, &CosmoInterpeter::threadStopped
	);

//	connect(
//			thread
//			, &qReal::interpretation::dataFlowInterpreter::CosmoThread::newThread
//			, this
//			, &CosmoInterpeter::newThread
//	);

//	connect(
//			thread
//			, &qReal::interpretation::dataFlowInterpreter::CosmoThread::killThread
//			, this
//			, &CosmoInterpeter::killThread
//	);

//	connect(
//			thread
//			, &qReal::interpretation::dataFlowInterpreter::CosmoThread::sendMessage
//			, this
//			, &CosmoInterpeter::sendMessage
//	);
}

//void CosmoInterpeter::killThread(const QString &threadId)
//{
//	if (mThreads.contains(threadId)) {
//		mThreads[threadId]->stop();
//	} else {
//		reportError(tr("Killing non-existent thread %1").arg(threadId));
//	}
//}

//void CosmoInterpeter::sendMessage(const QString &threadId, const QString &message)
//{
//	if (mThreads.contains(threadId)) {
//		mThreads[threadId]->newMessage(message);
//	}
//}

void CosmoInterpeter::connectToRobot()
{
	if (mState == interpreting) {
		return;
	}

	if (mRobotModelManager.model().connectionState() == RobotModelInterface::connectedState) {
		mRobotModelManager.model().stopRobot();
		mRobotModelManager.model().disconnectFromRobot();
	} else {
		mRobotModelManager.model().connectToRobot();
	}

	mActionConnectToRobot.setChecked(
			mRobotModelManager.model().connectionState() == RobotModelInterface::connectedState);
}

void CosmoInterpeter::reportError(const QString &message)
{
	mInterpretersInterface.errorReporter()->addError(message);
}
