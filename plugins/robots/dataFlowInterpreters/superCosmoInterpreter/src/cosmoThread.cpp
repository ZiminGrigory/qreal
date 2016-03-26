#include "cosmoThread.h"

#include <QtWidgets/QApplication>
#include <QtCore/QTimer>

#include <qrkernel/settingsManager.h>

#include <qrutils/interpreter/blocks/receiveThreadMessageBlock.h>

using namespace qReal;
using namespace interpretation;
using namespace interpreterCore;
using namespace interpreterCore::interpreter;
using namespace interpreterCore::interpreter::dataFlowInterpretation;

const int timeBeforeStart = 25;

CosmoThread::CosmoThread(const GraphicalModelAssistInterface *graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, details::BlocksTable *blocksTable
		, const IdList &initialNodeIds
		, const QString &threadId)
	: mGraphicalModelApi(graphicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mBlocksTable(blocksTable)
	, mId(threadId)
{
	init(initialNodeIds);
}

void CosmoThread::init(const IdList &initialNodeIds)
{
	for (auto &id: initialNodeIds) {
		mInitialNodes.append(dynamic_cast<DataFlowRobotsBlock *>(mBlocksTable->block(id)));
	}

	for (auto &node: mInitialNodes) {
		QTimer::singleShot(timeBeforeStart, this, [&]{node->interpretThis();});
	}
}

void CosmoThread::stop(qReal::interpretation::StopReason reason)
{
	emit stopped(reason);
}

void CosmoThread::failure()
{
	emit stopped(qReal::interpretation::StopReason::error);
}

void CosmoThread::error(const QString &message, const Id &source)
{
	mInterpretersInterface.errorReporter()->addError(message, source);
	failure();
}

QString CosmoThread::id() const
{
	return mId;
}
