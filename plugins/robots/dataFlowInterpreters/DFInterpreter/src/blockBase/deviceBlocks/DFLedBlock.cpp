#include "DFLedBlock.h"

using namespace dataFlowBlocks::details;

const QStringList leds = {"off", "green", "orange", "red"};

DFLedBlock::DFLedBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: LedDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["DATA"] = 2;
}

void DFLedBlock::init()
{
	mColor = stringProperty("ledColor");
}

void DFLedBlock::handleData(Led &led)
{
	if (hasNewData(portAssociatedWithProperty["CF_IN"])) {
		led.setColor(mColor);
		emit newDataInFlow(property(portAssociatedWithProperty["CF_IN"]), portAssociatedWithProperty["CF_OUT"]);
	} else {
		QVariant rawData = propertyFromPort("DATA");
		QString color = rawData.toString();
		if (leds.contains(color)) {
			led.setColor(color);
			emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
		} else {
			error("color should be one of : off, green, orange, red");
			return;
		}
	}
}
