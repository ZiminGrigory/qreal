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

#include "DFDrawPixelBlock.h"

using namespace dataFlowBlocks::details;

DFDrawPixelBlock::DFDrawPixelBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: DisplayDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["COORDS"] = 2;
}

void DFDrawPixelBlock::init()
{
	mRedraw = boolProperty("redraw");
}

void DFDrawPixelBlock::handleData(Display &display)
{
	if (hasNewData("COORDS"))
	{
		QVariantList config = propertyFromPort("COORDS").value<QVariantList>();
		if (config.size() != 2) {
			error("coords[] size for pixel should be equal to 2: x,y");
			return;
		}

		int x = config.at(0).toInt();
		int y = config.at(1).toInt();
		display.drawPixel(x,y);
	} else {
		propertyFromPort("CF_IN");
		const int x = eval<int>("XCoordinatePix");
		const int y = eval<int>("YCoordinatePix");
		display.drawPixel(x,y);
	}

	if (mRedraw) {
		display.redraw();
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
