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
	if (hasNewData("REPLACE_DATA")) {
		supressionStarted = true;
		startSupressionTime = QTime::currentTime();
		emit newDataInFlow(propertyFromPort("REPLACE_DATA"), portAssociatedWithProperty["OUT"]);
		propertyFromPort("DATA");
	} else if (hasNewData("TIME")) {
		timeToSupress = propertyFromPort("TIME").toInt();
	} else if (hasNewData("DATA")) {
		if (!supressionStarted || startSupressionTime.msecsTo(QTime::currentTime()) >= timeToSupress) {
			supressionStarted = false;
			emit newDataInFlow(propertyFromPort("DATA"), portAssociatedWithProperty["OUT"]);
		} else {
			propertyFromPort("DATA");
		}
	}
}
