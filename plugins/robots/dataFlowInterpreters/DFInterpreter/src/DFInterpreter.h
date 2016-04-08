/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QAction>
#include <QtCore/QMultiMap>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrutils/watchListWindow.h>
#include <qrutils/interpreter/thread.h>
#include <qrtext/languageToolboxInterface.h>

#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <kitBase/devicesConfigurationProvider.h>
#include <kitBase/interpreterInterface.h>

#include "interpreterCore/interpreter/details/blocksTable.h"
#include "interpreterCore/interpreter/details/sensorVariablesUpdater.h"
#include "interpreterCore/interpreter/details/autoconfigurer.h"

#include "DFThread.h"
#include "DFRobotsBlock.h"
#include "DFFactoryBase.h"
#include "DFRobotBlocksTableInterface.h"
#include "blockBase/DFSubprogramCall.h"


namespace interpreterCore {
namespace interpreter {
namespace dataFlowInterpretation {

/// Interprets robot diagram by executing blocks and sending commands to robot model. Manages models, connection,
/// threads, parser, can automatically configure robot by used blocks on diagram. It is the main class for
/// all interpretation subsystem.
class DFInterpeter : public kitBase::InterpreterInterface, public kitBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// Constructor,
	/// @param graphicalModelApi - reference to graphical model, main source of information about interpreted diagram.
	/// @param logicalModelApi - reference to logical model, which contains properties of blocks.
	/// @param interpretersInterface - reference to QReal main window. Used to highlight executing blocks and so on.
	/// @param projectManager - class that manages opening/saving diagrams, needed here to receive notifications
	///        about opening a new save file.
	/// @param blocksFactoryManager - blocks factory, needed to create "code behind" objects for blocks on diagram.
	/// @param robotModelManager - reference to currently selected robot model.
	/// @param parser - parser that is used to analyze and evaluate textual expressions inside properties of blocks.
	/// @param connectToRobotAction - reference to action that connects to robot, interpreter manages its state
	///        depending on success or failure of its own connection attempts.
	DFInterpeter(const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, const qReal::ProjectManagementInterface &projectManager
			, dataFlow::blocksBase::DFRobotBlocksFactoryInterface &factory
//			, BlocksFactoryManagerInterface &blocksFactoryManager
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &languageToolbox
			);

	~DFInterpeter() override;

	qReal::IdList supportedDiagrams() const;
public slots:
	void connectToRobot() override;
	void interpret() override;
	void stopRobot(qReal::interpretation::StopReason reason = qReal::interpretation::StopReason::userStop) override;
	int timeElapsed() const override;

private slots:
	void threadStopped(qReal::interpretation::StopReason reason);
//	void newThread(const qReal::Id &startBlockId, const QString &threadId);
//	void killThread(const QString &threadId);
//	void sendMessage(const QString &threadId, const QString &message);

	void connectedSlot(bool success, const QString &errorString);
	void devicesConfiguredSlot();

	void handleDataInFlow(const QVariant &data, int port);

	void handleStopRobot();

	qReal::IdList handleSubprogram(const qReal::Id &id, const qReal::Id &explosion);

signals:
	void dataToBlock(const QVector<QVariant> &data, int port);

private:
	enum InterpreterState {
		interpreting
		, waitingForDevicesConfiguredToLaunch
		, idle
	};

	qReal::IdList findStartChildrens(const qReal::Id &currentDiagramId);
	void initDiagramInterpetationProcess();
	void prepareDiagramInterpretation(const qReal::IdList &startElements, const QString &threadId);

	void addThread(DFThread * const thread, const QString &threadId);

	void reportError(const QString &message);


	const qReal::GraphicalModelAssistInterface &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;

	InterpreterState mState;
	quint64 mInterpretationStartedTimestamp;
	QHash<QString, DFThread *> mThreads;  // Has ownership
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	dataFlow::blocksBase::DFRobotBlocksFactoryInterface &mFactory;
	dataFlow::interpretation::DFRobotBlocksTableInterface *mBlocksTable;  // Has ownership

	using DataFlowRobotsBlock = dataFlow::interpretation::DFRobotsBlock;
	QMap<DataFlowRobotsBlock *, QMultiMap<int, QPair<DataFlowRobotsBlock *, int>>> connectResolver;
	QSet<qReal::Id> handledElements;
	QList<dataFlowBlocks::details::DFSubprogramCall *> mSubprograms;

	details::SensorVariablesUpdater mSensorVariablesUpdater;
	/// Reference to a parser to be able to clear parser state when starting interpretation.
	qrtext::LanguageToolboxInterface &mLanguageToolbox;
};

}
}
}
