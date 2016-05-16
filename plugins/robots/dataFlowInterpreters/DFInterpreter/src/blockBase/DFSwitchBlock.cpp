#include "DFSwitchBlock.h"

#include <QtCore/QPair>

using namespace dataFlowBlocks::details;

const QString dummyValue = "42";
const QList<QPair<QString, QString>> buckets = {{"cond0", "O0"}, {"cond1", "O1"}, {"cond2", "O2"}, {"cond3", "O3"}
		, {"cond4", "O4"}, {"cond5", "O5"}};

DFSwitchBlock::DFSwitchBlock()
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["DATA"] = 2;
	portAssociatedWithProperty["O0"] = 3;
	portAssociatedWithProperty["O1"] = 4;
	portAssociatedWithProperty["O2"] = 5;
	portAssociatedWithProperty["O3"] = 6;
	portAssociatedWithProperty["O4"] = 7;
	portAssociatedWithProperty["O5"] = 8;
}

void DFSwitchBlock::init()
{
	firstMatchOnly = boolProperty("sendOnlyFirst");

	for (const QPair<QString, QString> &b : buckets) {
		mConditions.append({b.first, stringProperty(b.first), b.second, stringProperty(b.second)});
	}
}


void DFSwitchBlock::handleData()
{
	if (!hasNewData("DATA") && !hasNewData("CF_IN")) {
		return;
	}

	/// @todo: check if activation by CF, but data is needed

	if (hasNewData("DATA")) {
		QVariant rawData = propertyFromPort("DATA");
		QString data = "data";

		setVariable(data, rawData);
	}

	for (const Bucket &b : mConditions) {
		bool condition = evalCode<bool>(b.conditionText, b.condition);
		if (condition) {
			QVariant expr = evalCode<QVariant>(b.expressionText, b.expression);
			emit newDataInFlow(expr, portAssociatedWithProperty[b.expression]);
		}

		if (firstMatchOnly) {
			break;
		}
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
