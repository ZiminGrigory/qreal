#include "DFVariable.h"


dataFlowBlocks::details::DFVariable::DFVariable()
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["IN"] = 1;
	portAssociatedWithProperty["CF_OUT"] = 2;
	portAssociatedWithProperty["OUT"] = 3;
}

void dataFlowBlocks::details::DFVariable::init()
{
	myVariables = stringProperty("variable").split(";");
}

void dataFlowBlocks::details::DFVariable::handleData()
{
	QString currentVar = stringProperty("variable");
	if (currentVar.contains('=')) {
		eval("variable");
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}

