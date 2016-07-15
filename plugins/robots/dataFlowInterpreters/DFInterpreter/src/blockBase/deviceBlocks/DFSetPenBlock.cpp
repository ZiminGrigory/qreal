#include "DFSetPenBlock.h"


using namespace dataFlowBlocks::details;


DFSetPenBlock::DFSetPenBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: MarkerDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["UP_DOWN"] = 2;
	portAssociatedWithProperty["COLOR"] = 3;
}

void DFSetPenBlock::init()
{
	mColor = propertyToColor(stringProperty("color"));
	isDownState = boolProperty("state");
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
	} else 	if (hasNewData("CF_IN")) {
		if (isDownState) {
			marker.down(mColor);
		} else {
			marker.up();
		}
	} else if (hasNewData("UP_DOWN")) {
		bool isDown= propertyFromPort("UP_DOWN").toBool();
		if (isDown) {
			marker.down(mColor);
		} else {
			marker.up();
		}
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
