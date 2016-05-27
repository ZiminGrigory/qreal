#include "DFInhibitionBlock.h"

using namespace dataFlowBlocks::details;


DFInhibitionBlock::DFInhibitionBlock()
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["STOP"] = 0;
		portAssociatedWithProperty["DATA"] = 1;
		portAssociatedWithProperty["OUT"] = 2;
		portAssociatedWithProperty["TIME"] = 3;
}

void DFInhibitionBlock::init()
{
	int time = intProperty("time");
	if (time) {
		timeToInhibit = time;
	}
}

void DFInhibitionBlock::handleData()
{
	if (hasNewData("STOP")) {
		inhibitionStarted = true;
		startInhibitionTime = QTime::currentTime();
		flushData();
	} else if (hasNewData("TIME")) {
		timeToInhibit = propertyFromPort("TIME").toInt();
	} else if (hasNewData("DATA")) {
		if (!inhibitionStarted || startInhibitionTime.msecsTo(QTime::currentTime()) >= timeToInhibit) {
			inhibitionStarted = false;
			emit newDataInFlow(propertyFromPort("DATA"), portAssociatedWithProperty["OUT"]);
		} else {
			propertyFromPort("DATA");
		}
	}
}

void DFInhibitionBlock::flushData()
{
	propertyFromPort("DATA"); // flush data
	propertyFromPort("STOP"); // flush data
}
