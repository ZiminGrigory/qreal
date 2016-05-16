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

	if (hasNewData(portAssociatedWithProperty["CF_IN"]) || hasNewData(portAssociatedWithProperty["DUMMY"])) {
		propertyFromPort("DUMMY");
		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_IN"]);
	}

	if (hasNewData(portAssociatedWithProperty["I0"])) {
		emit newDataInFlow(propertyFromPort("I0"), portAssociatedWithProperty["I0"]);
	}

	if (hasNewData(portAssociatedWithProperty["I1"])) {
		emit newDataInFlow(propertyFromPort("I1"), portAssociatedWithProperty["I1"]);
	}
}

