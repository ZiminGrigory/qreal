#include "DFSubprogramCall.h"

using namespace dataFlowBlocks::details;

DFSubprogramCall::DFSubprogramCall()
{
	portAssociatedWithProperty["DUMMY"] = -1;
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["I0"] = 1;
	portAssociatedWithProperty["O0"] = 2;
	portAssociatedWithProperty["I1"] = 3;
	portAssociatedWithProperty["O1"] = 4;
	portAssociatedWithProperty["CF_OUT"] = 5;
}

void DFSubprogramCall::handleData()
{
	if (!isActive) {
		isActive = true;
		emit firstActivation();
	}

	if (hasNewProperty(portAssociatedWithProperty["CF_IN"]) || hasNewProperty(portAssociatedWithProperty["DUMMY"])) {
		propertyFromPort("DUMMY");
		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_IN"]);
	}

	if (hasNewProperty(portAssociatedWithProperty["I0"])) {
		emit newDataInFlow(propertyFromPort("I0"), portAssociatedWithProperty["I0"]);
	}

	if (hasNewProperty(portAssociatedWithProperty["I1"])) {
		emit newDataInFlow(propertyFromPort("I1"), portAssociatedWithProperty["I1"]);
	}
}

