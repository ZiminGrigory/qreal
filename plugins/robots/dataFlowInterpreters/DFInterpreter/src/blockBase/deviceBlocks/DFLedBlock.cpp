#include "DFLedBlock.h"

using namespace dataFlowBlocks::details;

const QStringList leds = {"off", "green", "orange", "red"};

DFLedBlock::DFLedBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: LedDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["DATA"] = 1;
}


int DFLedBlock::activationPortNumber() const
{
	return portAssociatedWithProperty["DATA"];
}

void DFLedBlock::handleData(Led &led)
{
	QVariant rawData = propertyFromPort("DATA");

	if (!rawData.isNull()) { //check start activation
		QString color = rawData.toString();
		if (leds.contains(color)) {
			led.setColor(color);
		} else {
			error("color should be one of : off, green, orange, red");
			return;
		}
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
