#include "DFThread.h"

#include <QtWidgets/QApplication>
#include <QtCore/QTimer>

#include <qrkernel/settingsManager.h>

#include <qrutils/interpreter/blocks/receiveThreadMessageBlock.h>

using namespace qReal;
using namespace interpretation;
using namespace interpreterCore;
using namespace interpreterCore::interpreter;
using namespace interpreterCore::interpreter::dataFlowInterpretation;
using namespace dataFlow::interpretation;

const int timeBeforeStart = 25;
const int dummyPortForStarting = -1;


DFThread::DFThread(const GraphicalModelAssistInterface *graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, dataFlow::interpretation::DFRobotBlocksTableInterface *blocksTable
		, IdList initialNodeIds
		, const QString &threadId)
	: mGraphicalModelApi(graphicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mBlocksTable(blocksTable)
	, mInitialNodeIds(initialNodeIds)
	, mId(threadId)
{
}

void DFThread::start()
{
	init(mInitialNodeIds);
}

void DFThread::init(const IdList &initialNodeIds)
{
	for (auto &id: initialNodeIds) {
		mInitialNodes.append(mBlocksTable->block(id));
	}

	for (auto &node: mInitialNodes) {
		QTimer::singleShot(timeBeforeStart, [&](){
			node->handleNewDataFromFlow(QVariant(), dummyPortForStarting);});
	}
}

void DFThread::stop(qReal::interpretation::StopReason reason)
{
	emit stopped(reason);
}

void DFThread::failure()
{
	emit stopped(qReal::interpretation::StopReason::error);
}

void DFThread::error(const QString &message, const Id &source)
{
	mInterpretersInterface.errorReporter()->addError(message, source);
	failure();
}

QString DFThread::id() const
{
	return mId;
}
