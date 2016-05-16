#include "DFFunctionBlock.h"


using namespace dataFlowBlocks::details;

const QString dummyValue = "42";
const QStringList expressions = {"IO0", "IO1", "IO2", "IO3", "IO4", "IO5"};
const QStringList inputDataPorts = {"I0", "I1", "I2", "I3", "I4", "I5"};

dataFlowBlocks::details::DFFunctionBlock::DFFunctionBlock()
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["CF_IN"] = 0;
		portAssociatedWithProperty["CF_OUT"] = 1;
		portAssociatedWithProperty["I0"] = 2;
		portAssociatedWithProperty["I1"] = 4;
		portAssociatedWithProperty["I2"] = 6;
		portAssociatedWithProperty["I3"] = 8;
		portAssociatedWithProperty["I4"] = 10;
		portAssociatedWithProperty["I5"] = 12;
		portAssociatedWithProperty["O0"] = 3;
		portAssociatedWithProperty["O1"] = 5;
		portAssociatedWithProperty["O2"] = 7;
		portAssociatedWithProperty["O3"] = 9;
		portAssociatedWithProperty["O4"] = 11;
		portAssociatedWithProperty["O5"] = 13;
}

void DFFunctionBlock::init()
{
	isSynchronized = boolProperty("synch");

	mExpressions.append({"I0", "O0", "IO0", stringProperty("IO0")});
	mExpressions.append({"I1", "O1", "IO1", stringProperty("IO1")});
	mExpressions.append({"I2", "O2", "IO2", stringProperty("IO2")});
	mExpressions.append({"I3", "O3", "IO3", stringProperty("IO3")});
	mExpressions.append({"I4", "O4", "IO4", stringProperty("IO4")});
	mExpressions.append({"I5", "O5", "IO5", stringProperty("IO5")});
}

void DFFunctionBlock::handleData()
{
	/// @todo: check if it sends array

	if (!isSynchronized) {
		for (const Bucket &b : mExpressions) {
			if (hasNewData(b.input)) {
				setVariable(b.input, propertyFromPort(b.input));
				QVariant data = evalCode<QVariant>(b.expressionText, b.expression);
				emit newDataInFlow(data, portAssociatedWithProperty[b.output]);
				break;
			}
		}

		return;
	}

	for (const Bucket &b : mExpressions) {
		if (hasNewData(b.input)) {
			setVariable(b.input, propertyFromPort(b.input));
		} else {
			setVariable(b.input, dummyValue);
		}

		QVariant data = evalCode<QVariant>(b.expressionText, b.expression);
		emit newDataInFlow(data, portAssociatedWithProperty[b.output]);
	}
}

bool DFFunctionBlock::isSynchronizationNeeded()
{
	return property("synch").toBool();
}


