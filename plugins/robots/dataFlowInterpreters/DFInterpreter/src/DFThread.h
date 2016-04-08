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
#include <QtCore/QStack>
#include <QtCore/QQueue>
#include <QtCore/QSignalMapper>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

#include <qrutils/interpreter/blockInterface.h>
#include <qrutils/interpreter/stopReason.h>
#include <qrutils/utilsDeclSpec.h>

#include <interpreterCore/interpreter/details/blocksTable.h>

#include "DFRobotsBlock.h"
#include "DFRobotBlocksFactoryInterface.h"
#include "DFRobotBlocksTableInterface.h"

class QTimer;

namespace interpreterCore {
namespace interpreter {
namespace dataFlowInterpretation {

/// Program execution thread. Has currently executed block, and its own stack.
class DFThread : public QObject
{
	Q_OBJECT

public:
	/// Creates new instance of thread starting from specified blocks.
	/// @param graphicalModelApi - graphical model, contains diagram.
	/// @param interpretersInterface - reference to QReal main window.
	/// @param initialNodeType - the type of the element to start on diagram when stepping into it.
	/// @param blocksTable - interpreter-wide table of blocks (map from ids to "code-behind" objects).
	/// @param initialNode - node that shall be executed first in this thread.
	DFThread(const qReal::GraphicalModelAssistInterface *graphicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, dataFlow::interpretation::DFRobotBlocksTableInterface *blocksTable
			, qReal::IdList initialNodeIds
			, const QString &threadId);


	/// Stops interpretation. Optionally reason may be specified.
	void stop(qReal::interpretation::StopReason reason = qReal::interpretation::StopReason::finised);

	/// Returns string id of a thread.
	QString id() const;

public slots:
	void start();

signals:
	/// Emitted when interpretation process was terminated (correctly or due to errors).
	void stopped(qReal::interpretation::StopReason reason);

	/// Emitted when one of the blocks interpreted by this thread requested new thread.
	void newThread(const qReal::Id &startBlockId, const QString &threadId);

	/// Emitted when one of the blocks wants to stop some thread.
	void killThread(const QString &threadId);

	/// Emitted when one of the blocks wants to send message to another thread.
	void sendMessage(const QString &threadId, const QString &message);

private slots:
	void failure();

private:
	void init(const qReal::IdList &initialNodeIds);

	void error(const QString &message, const qReal::Id &source = qReal::Id());

	const qReal::GraphicalModelAssistInterface *mGraphicalModelApi;  // Doesn't have ownership
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	dataFlow::interpretation::DFRobotBlocksTableInterface *mBlocksTable;
	const qReal::Id mInitialDiagram;
	qReal::IdList mInitialNodeIds;
	QString mId;
	QList<dataFlow::interpretation::DFRobotsBlockInterface *> mInitialNodes;
	QQueue<QString> mMessages;
};

}
}
}
