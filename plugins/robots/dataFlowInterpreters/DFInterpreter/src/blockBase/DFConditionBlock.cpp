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
	condition = stringProperty("condition");
}

void DFConditionBlock::handleData()
{
	if (hasNewProperty("VARS")) {
		vars.replace("{}", qVariantListToLuaArrayInitializeList(propertyFromPort("VARS").value<QVariantList>()));
	}

	if (hasNewProperty("DATA")) {
		QVariant rawData = propertyFromPort("DATA");
		QString data = "data = ";

		if (rawData.canConvert<QVariantList>()) {
			data += qVariantListToLuaArrayInitializeList(rawData.value<QVariantList>());
		} else {
			data += rawData.toString();
		}

		bool cond = evalCode<bool>(data + ";" + vars + ";" + condition, "condition");

		if (cond) {
			emit newDataInFlow(rawData, portAssociatedWithProperty["TRUE"]);
		} else {
			emit newDataInFlow(rawData, portAssociatedWithProperty["FALSE"]);
		}
	}
}
