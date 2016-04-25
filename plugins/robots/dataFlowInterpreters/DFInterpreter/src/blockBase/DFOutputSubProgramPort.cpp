#include "DFOutputSubProgramPort.h"

using namespace dataFlowBlocks::details;



DFOutputSubProgramPort::DFOutputSubProgramPort()
{
	portAssociatedWithProperty["DATA"] = 0;
	portAssociatedWithProperty["O0"] = 2;
	portAssociatedWithProperty["O1"] = 4;
	portAssociatedWithProperty["CF_OUT"] = 5;
}

void DFOutputSubProgramPort::handleData()
{
	emit newDataInFlow(propertyFromPort("DATA"), portAssociatedWithProperty[property("identifier").toString()]);
}


