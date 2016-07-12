#include "DFConditionBlock.h"

using namespace dataFlowBlocks::details;

DFConditionBlock::DFConditionBlock()
	: condition()
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["DATA"] = 1;
	portAssociatedWithProperty["TRUE"] = 2;
	portAssociatedWithProperty["FALSE"] = 3;
}

void DFConditionBlock::init()
{
	condition = stringProperty("condition");
}

void DFConditionBlock::handleData()
{
	if (hasNewData("CF_IN")) {
		bool cond = evalCode<bool>(condition, "condition");
		if (errorsOccured()) {
			error("If block cannot parse condition");
		} else {
			if (cond) {
				emit newDataInFlow(propertyFromPort("CF_IN"), portAssociatedWithProperty["TRUE"]);
			} else {
				emit newDataInFlow(propertyFromPort("CF_IN"), portAssociatedWithProperty["FALSE"]);
			}
		}
	}

	if (hasNewData("DATA")) {
		QVariant rawData = propertyFromPort("DATA");
		QString data = "data";
		setVariable(data, rawData);

		bool cond = evalCode<bool>(condition, "condition");

		if (cond) {
			emit newDataInFlow(rawData, portAssociatedWithProperty["TRUE"]);
		} else {
			emit newDataInFlow(rawData, portAssociatedWithProperty["FALSE"]);
		}
	}
}
