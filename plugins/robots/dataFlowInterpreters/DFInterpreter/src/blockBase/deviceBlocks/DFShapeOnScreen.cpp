/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "DFShapeOnScreen.h"

#include <trikKit/robotModel/parts/trikDisplay.h>

using namespace dataFlowBlocks::details;

const QStringList shapeTypes = {"arc", "ellipse", "rectangle", "line", "point"};

DFShapeOnScreen::DFShapeOnScreen(kitBase::robotModel::RobotModelInterface &robotModel)
	: DisplayDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["CONFIG"] = 1;
	portAssociatedWithProperty["SHAPE"] = 2;
}

int DFShapeOnScreen::activationPortNumber() const
{
	return portAssociatedWithProperty["SHAPE"];
}

void DFShapeOnScreen::init()
{
	mType = stringProperty("shapeType");
	mRedraw = boolProperty("Redraw");
}

void DFShapeOnScreen::handleData(Display &display)
{

	if (hasNewData("SHAPE")) {
		QString rawText = propertyFromPort("SHAPE").toString();
		if (shapeTypes.contains(rawText)) {
			mType = rawText;
		} else {
			error("shape type is incorrect");
			return;
		}
	}

	if (hasNewData("CONFIG")) {
		QVariantList config = propertyFromPort("CONFIG").value<QVariantList>();

		if (mType == "point") {
			if (config.size() != 2) {
				error("config[] size for point should be equal to 2: x,y");
				return;
			}

			int x = config.at(0).toInt();
			int y = config.at(1).toInt();

			display.drawPixel(x, y);
		} else if (mType == "line") {
			if (config.size() != 4) {
				error("config[] size for line should be equal to 4: x1,y1,x2,y2");
				return;
			}

			int x1 = config.at(0).toInt();
			int y1 = config.at(1).toInt();
			int x2 = config.at(2).toInt();
			int y2 = config.at(3).toInt();

			display.drawLine(x1, y1, x2, y2);
		} else if (mType == "rectangle") {
			if (config.size() != 4) {
				error("config[] size for rectangle should be equal to 4:x,y,width,height");
				return;
			}

			int x = config.at(0).toInt();
			int y = config.at(1).toInt();
			int width = config.at(2).toInt();
			int height = config.at(3).toInt();

			display.drawRect(x, y, width, height);
		} else if (mType == "ellipse") {
			if (config.size() != 4) {
				error("config[] size for ellipse should be equal to 4:x,y,width,height");
				return;
			}

			auto trikDisplay = static_cast<trik::robotModel::parts::TrikDisplay *>(&display);

			int x = config.at(0).toInt();
			int y = config.at(1).toInt();
			int width = config.at(2).toInt();
			int height = config.at(3).toInt();

			trikDisplay->drawEllipse(x, y, width, height);
		} else if (mType == "arc") {
			if (config.size() != 6) {
				error("config[] size for arc should be equal to 6:x,y,width,height,startAngle,spanAngle");
				return;
			}

			auto trikDisplay = static_cast<trik::robotModel::parts::TrikDisplay *>(&display);

			int x = config.at(0).toInt();
			int y = config.at(1).toInt();
			int width = config.at(2).toInt();
			int height = config.at(3).toInt();
			int startAngle = config.at(4).toInt();
			int spanAngle = config.at(5).toInt();

			trikDisplay->drawArc(x, y, width, height, startAngle, spanAngle);
		}


		if (mRedraw) {
			display.redraw();
		}

		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
	}
}
