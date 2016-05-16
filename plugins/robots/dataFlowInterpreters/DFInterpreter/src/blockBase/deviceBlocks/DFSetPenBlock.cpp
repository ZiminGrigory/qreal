#include "DFSetPenBlock.h"


using namespace dataFlowBlocks::details;


DFSetPenBlock::DFSetPenBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: MarkerDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["UP_DOWN"] = 1;
	portAssociatedWithProperty["COLOR"] = 2;
}

int DFSetPenBlock::activationPortNumber()
{
	portAssociatedWithProperty["UP_DOWN"];
}

void DFSetPenBlock::init()
{
	mColor = propertyToColor(stringProperty("color"));
}

void DFSetPenBlock::handleData(Marker &marker)
{
	if (hasNewData("COLOR")) {

		QString rawColor = propertyFromPort("COLOR").toString();

		mColor = propertyToColor(rawColor);
		if (!mColor.isValid()) {
			error(QString("Use correct colors, %1 is incorrect").arg(rawColor));
			return;
		}

		if (marker.isDown()) {
			marker.down(mColor);
		}
	}

	if (hasNewData("UP_DOWN")) {
		bool isDown= propertyFromPort("UP_DOWN").toBool();
		if (isDown) {
			marker.down(mColor);
		} else {
			marker.up();
		}
	}
}
