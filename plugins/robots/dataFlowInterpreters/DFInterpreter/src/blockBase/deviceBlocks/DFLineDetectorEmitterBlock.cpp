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

#include "DFLineDetectorEmitterBlock.h"


#include <kitBase/robotModel/robotParts/vectorSensor.h>


using namespace dataFlowBlocks::details;

DFLineDetectorEmitterBlock::DFLineDetectorEmitterBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: LineSensorDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["OUT"] = 1;
	portAssociatedWithProperty["TYPE"] = 2;
}

void DFLineDetectorEmitterBlock::init()
{
	mCameraMode = stringProperty("type");
	if (mCameraMode != "Line") {
		error("other modes aren't available now");
		return;
	}
}

void DFLineDetectorEmitterBlock::handleData(dataFlowBlocks::details::LineSensor &lineSensor)
{
	if (hasNewData("TYPE") && propertyFromPort("TYPE").toString() != "Line") {
		error("other modes aren't available now");
		return;
	} else if (hasNewData("CF_IN") && mCameraMode == "Line") {
		mSensor = &lineSensor;
		QVariant owner;
		owner.setValue<DFLineDetectorEmitterBlock *>(this);
		mSensor->setProperty("owner", owner);
		propertyFromPort("CF_IN");
		disconnect(mSensor, &kitBase::robotModel::robotParts::VectorSensor::newData
				, this, &DFLineDetectorEmitterBlock::newVectorDataReceived);
		connect(mSensor, &kitBase::robotModel::robotParts::VectorSensor::newData
				, this, &DFLineDetectorEmitterBlock::newVectorDataReceived, Qt::QueuedConnection);
	}
}

void DFLineDetectorEmitterBlock::newVectorDataReceived(QVector<int> data)
{
	QVariant output = vectorToQVariantList(data);
	if (mSensor->property("owner").value<DFLineDetectorEmitterBlock *>() == this) {
		emit newDataInFlow(output, portAssociatedWithProperty["OUT"]);
	}
}
