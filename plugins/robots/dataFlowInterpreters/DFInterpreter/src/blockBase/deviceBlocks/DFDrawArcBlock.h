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

#pragma once

#include "plugins/robots/dataFlowInterpreters/DFInterpreter/src/blockBase/DFDeviceBlock.h"

#include <kitBase/robotModel/robotParts/display.h>

namespace dataFlowBlocks {
namespace details {

using DisplayDevice = DFDeviceBlock<kitBase::robotModel::robotParts::Display>;
using Display = kitBase::robotModel::robotParts::Display;

class DFDrawArcBlock : public DisplayDevice
{
	Q_OBJECT

public:
	explicit DFDrawArcBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	void init() override;
	void handleData(Display &display) override;

private:
	bool mRedraw = false;
};

}
}
