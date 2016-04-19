#include "DFSeparatorBlock.h"

dataFlowBlocks::details::DFSeparatorBlock::DFSeparatorBlock()
{
	portAssociatedWithProperty["DATA"] = 0;
	portAssociatedWithProperty["OUT1"] = 1;
	portAssociatedWithProperty["OUT2"] = 2;
}

/// @todo: more than 2

void dataFlowBlocks::details::DFSeparatorBlock::handleData()
{
	QVariantList data = propertyFromPort("DATA").value<QVariantList>();
	emit newDataInFlow(data.first(), portAssociatedWithProperty["OUT1"]);
	data.pop_front();
	emit newDataInFlow(data.first(), portAssociatedWithProperty["OUT2"]);
}
