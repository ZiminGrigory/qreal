#include "DFInputSubProgramPort.h"

using namespace dataFlowBlocks::details;


DFInputSubProgramPort::DFInputSubProgramPort()
{
	portAssociatedWithProperty["OUT"] = 0;
}

QString DFInputSubProgramPort::idToPort()
{
	return property("identifier").toString();
}

void DFInputSubProgramPort::handleData()
{
	emit newDataInFlow(propertyFromPort("OUT"), portAssociatedWithProperty["OUT"]);
}


