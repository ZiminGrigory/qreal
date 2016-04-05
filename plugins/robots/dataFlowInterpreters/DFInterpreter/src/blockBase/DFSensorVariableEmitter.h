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
#include <kitBase/robotModel/robotParts/scalarSensor.h>
#include <kitBase/robotModel/robotParts/vectorSensor.h>


#include "plugins/robots/dataFlowInterpreters/DFInterpreter/src/DFRobotsBlock.h"


namespace dataFlowBlocks {
namespace details {

/// block for data from sensors
class DFSensorVariableEmitter : public dataFlow::interpretation::DFRobotsBlock
{
	Q_OBJECT

public:
	DFSensorVariableEmitter(kitBase::robotModel::RobotModelInterface &robotModel);
	void configure() override;

protected:
	void handleData() override;

private slots:
	void newIntDataReceived(int data);
	void newVectorDataReceived(QVector<int> data);

private:
	kitBase::robotModel::RobotModelInterface &mRobotModel;
	kitBase::robotModel::robotParts::ScalarSensor *mScalarSensor;
	kitBase::robotModel::robotParts::VectorSensor *mVectorSensor;
};

}
}
