#include "DFPaintSettingsBlock.h"


#include "trikKit/robotModel/parts/trikDisplay.h"


using namespace dataFlowBlocks::details;
DFPaintSettingsBlock::DFPaintSettingsBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: DisplayDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["BRUSH_COLOR"] = 2;
	portAssociatedWithProperty["BRUSH_WIDTH"] = 3;
	portAssociatedWithProperty["BACKGROUND_COLOR"] = 4;
}

void DFPaintSettingsBlock::init()
{
	mBrushColor = propertyToColor(stringProperty("color"));
	mBrushWidth = intProperty("width");
	mBackgroundColor = propertyToColor(stringProperty("backgroundColor"));

	if (!mBackgroundColor.isValid()) {
		error(QString("Use correct colors, %1 is incorrect").arg(stringProperty("backgroundColor")));
	}

	if (!mBrushColor.isValid()) {
		error(QString("Use correct colors, %1 is incorrect").arg(stringProperty("color")));
	}

	if (mBrushWidth <= 0) {
		error(QString("brush width should be greater then 0"));
	}
}

void DFPaintSettingsBlock::handleData(Display &display)
{
	auto trikDisplay = static_cast<trik::robotModel::parts::TrikDisplay *>(&display);


	if (hasNewData("CF_IN")) {
		trikDisplay->setBackground(mBackgroundColor);
		trikDisplay->setPainterWidth(mBrushWidth);
		trikDisplay->setPainterColor(mBrushColor);
	}

	if (hasNewData("BRUSH_COLOR")) {
		QString rawColor = propertyFromPort("COLOR").toString();
		mBrushColor = propertyToColor(rawColor);
		if (!mBrushColor.isValid()) {
			error(QString("Use correct colors, %1 is incorrect").arg(rawColor));
			return;
		}

		trikDisplay->setPainterColor(mBrushColor);
	}

	if (hasNewData("BRUSH_WIDTH")) {
		mBrushWidth = propertyFromPort("BRUSH_WIDTH").toInt();
		if (mBrushWidth <= 0) {
			error(QString("brush width should be greater then 0"));
		}

		trikDisplay->setPainterWidth(mBrushWidth);
	}

	if (hasNewData("BACKGROUND_COLOR")) {
		QString rawColor = propertyFromPort("BACKGROUND_COLOR").toString();
		mBackgroundColor = propertyToColor(rawColor);
		if (!mBackgroundColor.isValid()) {
			error(QString("Use correct colors, %1 is incorrect").arg(rawColor));
			return;
		}

		trikDisplay->setBackground(mBackgroundColor);
	}

	if (boolProperty("Redraw")) {
		trikDisplay->redraw();
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
