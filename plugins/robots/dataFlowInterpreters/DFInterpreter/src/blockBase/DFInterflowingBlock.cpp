#include "DFInterflowingBlock.h"


using namespace dataFlowBlocks::details;

DFInterflowingBlock::DFInterflowingBlock()
{
	portAssociatedWithProperty["DATA1"] = 0;
	portAssociatedWithProperty["DATA2"] = 2;
	portAssociatedWithProperty["OUT"] = 1;
}

bool DFInterflowingBlock::isSynchronizationNeeded()
{
	return true;
}

void DFInterflowingBlock::configureSynchronizedPorts()
{
	synchronisedPorts.insert(portAssociatedWithProperty["DATA1"]);
	synchronisedPorts.insert(portAssociatedWithProperty["DATA2"]);
}

void DFInterflowingBlock::handleData()
{
	const QVariantList list = {propertyFromPort("DATA1"), propertyFromPort("DATA2")};
	emit newDataInFlow(list, portAssociatedWithProperty["OUT"]);
}
