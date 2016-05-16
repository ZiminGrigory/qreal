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

#pragma once

#include "plugins/robots/dataFlowInterpreters/DFInterpreter/src/blockBase/DFDeviceBlock.h"

#include <twoDModel/robotModel/parts/marker.h>

namespace dataFlowBlocks {
namespace details {

using Marker = twoDModel::robotModel::parts::Marker;
using MarkerDevice = DFDeviceBlock<Marker>;

class DFSetPenBlock : public MarkerDevice
{
	Q_OBJECT

public:
	explicit DFSetPenBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	void init() override;
	void handleData(Marker &marker) override;

private:
	QColor mColor = QColor(Qt::black);
};

}
}
