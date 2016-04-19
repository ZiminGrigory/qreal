#include "DFConditionBlock.h"

using namespace dataFlowBlocks::details;

DFConditionBlock::DFConditionBlock()
	: condition()
	, vars("vars = {}")
{
//	QMap<int, QVariant> valueOnPort;
//	QMap<QString, int> portAssociatedWithProperty;
	portAssociatedWithProperty["DATA"] = 0;
	portAssociatedWithProperty["VARS"] = 1;
	portAssociatedWithProperty["TRUE"] = 2;
	portAssociatedWithProperty["FALSE"] = 3;
}

void DFConditionBlock::init()
{
	setVariable(vars, QVariantList());
	condition = stringProperty("condition");
}

void DFConditionBlock::handleData()
{
	if (hasNewProperty("VARS")) {
		setVariable(vars, propertyFromPort("VARS"));
	}

	if (hasNewProperty("DATA")) {
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
