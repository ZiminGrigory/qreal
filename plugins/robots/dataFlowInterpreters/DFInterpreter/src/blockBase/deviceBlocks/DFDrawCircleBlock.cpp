/* Copyright 2007-2016 QReal Research Group
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

#include "DFDrawCircleBlock.h"

#include <trikKit/robotModel/parts/trikDisplay.h>

using namespace dataFlowBlocks::details;

DFDrawCircleBlock::DFDrawCircleBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: DisplayDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["COORDS"] = 2;
}

void DFDrawCircleBlock::init()
{
	mRedraw = boolProperty("redraw");
}

void DFDrawCircleBlock::handleData(Display &display)
{
	auto trikDisplay = dynamic_cast<trik::robotModel::parts::TrikDisplay *>(&display);

	if (hasNewData("COORDS"))
	{
		QVariantList config = propertyFromPort("COORDS").value<QVariantList>();
		if (config.size() != 4) {
			error("coords[] size for circle should be equal to 4: x,y,w,h");
			return;
		}

		const int x = config.at(0).toInt();
		const int y = config.at(1).toInt();
		const int w = config.at(2).toInt();
		const int h = config.at(3).toInt();
		const bool filled = propertyExist("Filled") ? boolProperty("Filled") : false;
		trikDisplay->drawEllipse(x, y, w, h, filled);
	} else {
		propertyFromPort("CF_IN");
		const int x = eval<int>("XCoordinatePix");
		const int y = eval<int>("YCoordinatePix");
		const int w = eval<int>("Width");
		const int h = eval<int>("Height");
		const bool filled = propertyExist("Filled") ? boolProperty("Filled") : false;
		trikDisplay->drawEllipse(x, y, w, h, filled);
	}

	if (mRedraw) {
		display.redraw();
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
