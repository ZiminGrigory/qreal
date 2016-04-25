#include "DFSupressorBlock.h"

using namespace dataFlowBlocks::details;

DFSupressorBlock::DFSupressorBlock()
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["REPLACE_DATA"] = 0;
		portAssociatedWithProperty["DATA"] = 1;
		portAssociatedWithProperty["OUT"] = 2;
		portAssociatedWithProperty["TIME"] = 3;
}

void DFSupressorBlock::init()
{
	int time = intProperty("time");
	if (time) {
		timeToSupress = time;
	}
}

void DFSupressorBlock::handleData()
{
	if (!supressionStarted) {
		if (hasNewProperty("REPLACE_DATA")) {
			supressionStarted = true;
			startSupressionTime = QTime::currentTime();
			emit newDataInFlow(propertyFromPort("REPLACE_DATA"), portAssociatedWithProperty["OUT"]);
			flushData();
			return;
		}

		if (hasNewProperty("TIME")) {
			timeToSupress = propertyFromPort("TIME").toInt();
		}

		if (hasNewProperty("DATA")) {
			emit newDataInFlow(propertyFromPort("DATA"), portAssociatedWithProperty["OUT"]);
		}

	} else {
		if (startSupressionTime.msecsTo(QTime::currentTime()) >= timeToSupress) {
			supressionStarted = false;
			handleData();
		} else if (hasNewProperty("REPLACE_DATA")) {
			emit newDataInFlow(propertyFromPort("REPLACE_DATA"), portAssociatedWithProperty["OUT"]);
			flushData();
		}
	}
}

void DFSupressorBlock::flushData()
{
	propertyFromPort("DATA"); // flush data
	propertyFromPort("TIME"); // flush data
}
