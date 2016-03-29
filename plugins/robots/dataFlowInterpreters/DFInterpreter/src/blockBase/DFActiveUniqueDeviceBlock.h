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

#include <kitBase/robotModel/robotModelInterface.h>
#include <kitBase/robotModel/robotModelUtils.h>

#include "plugins/robots/dataFlowInterpreters/superCosmoInterpreter/src/DFRobotsBlock.h"


namespace dataFlowBlocks {
namespace details {

/// A base for all blocks that work with some robot`s device. Block which can be represented by many instance,
/// but only one is actually may be active on diagram. This block will
/// perform search of some device of the given type and call doJob() from it if it was found
/// or display an error otherwise.
template<typename Device>
class DFActiveUniqueDeviceBlock : public interpreterCore::interpreter::dataFlowInterpretation::DFRobotsBlock
{

public:
	explicit DFActiveUniqueDeviceBlock(kitBase::robotModel::RobotModelInterface &robotModel)
		: mRobotModel(robotModel)
	{
	}

	void run() override
	{
		const kitBase::robotModel::DeviceInfo deviceInfo = kitBase::robotModel::DeviceInfo::create<Device>();
		const QString port = deviceInfo.name()[0].toUpper() + deviceInfo.name().mid(1) + "Port";
		Device * const device = kitBase::robotModel::RobotModelUtils::findDevice<Device>(mRobotModel, port);
		if (device) {
			// property allow use all representation of device block sequantally
			this->setProperty("owner", this);
			doJob(*device);
		} else {
			error(QObject::tr("%1 is not configured.").arg(deviceInfo.friendlyName()));
		}
	}

protected:
	/// Implementation may consider that the device is configured and ready to work.
	virtual void doJob(Device &display) = 0;

private:
	kitBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
