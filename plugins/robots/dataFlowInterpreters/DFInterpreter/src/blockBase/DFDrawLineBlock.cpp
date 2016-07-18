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

#include "DFDrawLineBlock.h"

using namespace dataFlowBlocks::details;

DFDrawLineBlock::DFDrawLineBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: DisplayDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["COORDS"] = 2;
}

void DFDrawLineBlock::init()
{
	mRedraw = boolProperty("redraw");
}

void DFDrawLineBlock::handleData(Display &display)
{
	if (hasNewData("COORDS"))
	{
		QVariantList config = propertyFromPort("COORDS").value<QVariantList>();
		if (config.size() != 4) {
			error("coords[] size for line should be equal to 4: x1,y1,x2,y2");
			return;
		}

		int x1 = config.at(0).toInt();
		int y1 = config.at(1).toInt();
		int x2 = config.at(2).toInt();
		int y2 = config.at(3).toInt();

		display.drawLine(x1, y1, x2, y2);
	} else {
		propertyFromPort("CF_IN");
		const int x1 = eval<int>("X1CoordinatePix");
		const int y1 = eval<int>("Y1CoordinatePix");
		const int x2 = eval<int>("X2CoordinatePix");
		const int y2 = eval<int>("Y2CoordinatePix");
		display.drawLine(x1, y1, x2, y2);
	}

	if (mRedraw) {
		display.redraw();
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
