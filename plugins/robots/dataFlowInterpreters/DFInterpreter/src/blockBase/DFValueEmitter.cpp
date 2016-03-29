#include "DFValueEmitter.h"

dataFlowBlocks::details::DFValueEmitter::DFValueEmitter()
{
//	QMap<int, QVariant> valueOnPort;
//	QMap<QString, int> portAssociatedWithProperty;
	portAssociatedWithProperty["CF_IN"] = 1;
	portAssociatedWithProperty["OUT"] = 2;
}

void dataFlowBlocks::details::DFValueEmitter::run()
{
	emit newDataInFlow(QVariant(Block::property("varValue")), portAssociatedWithProperty["OUT"]);
}
