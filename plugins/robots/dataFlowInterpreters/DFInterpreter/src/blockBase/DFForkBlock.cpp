#include "DFForkBlock.h"

dataFlowBlocks::details::DFForkBlock::DFForkBlock()
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["OUT"] = 1;
}

void dataFlowBlocks::details::DFForkBlock::handleData()
{
	if (hasNewData("CF_IN")) {
		property("CF_IN");
		emit newDataInFlow(QVariant(), portAssociatedWithProperty["OUT"]);
	}
}
