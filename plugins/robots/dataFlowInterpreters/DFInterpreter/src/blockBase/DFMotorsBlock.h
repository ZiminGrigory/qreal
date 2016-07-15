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
#include <kitBase/robotModel/robotParts/motor.h>
#include <kitBase/robotModel/robotParts/motorsAggregator.h>

#include "plugins/robots/dataFlowInterpreters/DFInterpreter/src/DFRobotsBlock.h"


namespace dataFlowBlocks {
namespace details {

/// A base for all blocks that work with motors.
class DFMotorsBlock : public dataFlow::interpretation::DFRobotsBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit DFMotorsBlock(kitBase::robotModel::RobotModelInterface &robotModel);

	QMap<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> usedDevices();

protected:
	void init() override;
	void handleData() override;
	bool isSynchronizationNeeded() override;

	kitBase::robotModel::RobotModelInterface &mRobotModel;

private:
	template<class MotorType>
	QList<MotorType *> parsePorts(ReportErrors reportErrors = ReportErrors::report)
	{
		QList<MotorType *> result;
		const QList<kitBase::robotModel::PortInfo> ports = parsePorts(reportErrors);

		for (const kitBase::robotModel::PortInfo &port : ports) {
			MotorType * const motor = kitBase::robotModel::RobotModelUtils::findDevice<MotorType>(mRobotModel, port);
			if (motor) {
				result << motor;
			}
		}

		return result;
	}

	QMap<QString, int> mPreviousPower;

	QList<kitBase::robotModel::PortInfo> parsePorts(ReportErrors reportErrors = ReportErrors::report);

	QMap<QString ,kitBase::robotModel::robotParts::Motor *> motors;
};

}
}

