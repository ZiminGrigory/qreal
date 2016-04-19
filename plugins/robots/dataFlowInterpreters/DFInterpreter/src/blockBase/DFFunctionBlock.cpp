#include "DFFunctionBlock.h"


using namespace dataFlowBlocks::details;

const QString dummyValue = "42";
const QStringList expressions = {"IO0", "IO1", "IO3", "IO4", "IO5"};

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

void DFFunctionBlock::handleData()
{
	/// @todo: check if it sends array
	QVariant expression0 = evalCode<QVariant>(prepareCode("I0", "IO0"), "IO0");
	QVariant expression1 = evalCode<QVariant>(prepareCode("I1", "IO1"), "IO1");
	QVariant expression2 = evalCode<QVariant>(prepareCode("I2", "IO2"), "IO2");
	QVariant expression3 = evalCode<QVariant>(prepareCode("I3", "IO3"), "IO3");
	QVariant expression4 = evalCode<QVariant>(prepareCode("I3", "IO4"), "IO4");
	QVariant expression5 = evalCode<QVariant>(prepareCode("I5", "IO5"), "IO5");


	emit newDataInFlow(expression0, portAssociatedWithProperty["O0"]);
	emit newDataInFlow(expression1, portAssociatedWithProperty["O1"]);
	emit newDataInFlow(expression2, portAssociatedWithProperty["O2"]);
	emit newDataInFlow(expression3, portAssociatedWithProperty["O3"]);
	emit newDataInFlow(expression4, portAssociatedWithProperty["O4"]);
	emit newDataInFlow(expression5, portAssociatedWithProperty["O5"]);
}


QString DFFunctionBlock::prepareCode(const QString &port, const QString &propertyName)
{
	QVariant rawData = propertyFromPort(port);
	QString value = "";

	if (rawData.canConvert<QVariantList>()) {
		value += qVariantListToLuaArrayInitializeList(rawData.value<QVariantList>());
	} else {
		value += rawData.toString();
	}

	QString res = port + "=" + (value != "" ? value : dummyValue) + ";" + stringProperty(propertyName);
	return res;
}

