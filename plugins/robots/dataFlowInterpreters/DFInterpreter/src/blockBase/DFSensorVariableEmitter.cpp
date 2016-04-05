#include "DFSensorVariableEmitter.h"


#include <kitBase/robotModel/robotModelUtils.h>

using namespace dataFlowBlocks::details;
using namespace kitBase::robotModel;

DFSensorVariableEmitter::DFSensorVariableEmitter(RobotModelInterface &robotModel)
	: DFRobotsBlock()
	, mRobotModel(robotModel)
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["CF_IN"] = 0;
		portAssociatedWithProperty["OUT"] = 1;
}

void DFSensorVariableEmitter::handleData()
{
	if (mScalarSensor) {
		connect(mScalarSensor, &robotParts::ScalarSensor::newData
				, this, &DFSensorVariableEmitter::newIntDataReceived, Qt::QueuedConnection);
	}

	if (mVectorSensor) {
		connect(mVectorSensor, &robotParts::VectorSensor::newData
				, this, &DFSensorVariableEmitter::newVectorDataReceived, Qt::QueuedConnection);
	}
}

void DFSensorVariableEmitter::configure()
{
	const QList<PortInfo> localAvailablePorts = mRobotModel.configurablePorts();
	const QString selectedVar = stringProperty("VarName");
	for (const PortInfo &info : localAvailablePorts) {
		if (info.reservedVariable() == selectedVar) {
			auto localDevice = RobotModelUtils::findDevice<robotParts::AbstractSensor>(mRobotModel, info);
			mScalarSensor = dynamic_cast<robotParts::ScalarSensor *>(localDevice);
			mVectorSensor = dynamic_cast<robotParts::VectorSensor *>(localDevice);
			break;
		}
	}
}

void DFSensorVariableEmitter::newIntDataReceived(int data)
{
	emit newDataInFlow(QVariant(data), portAssociatedWithProperty["OUT"]);
}

void DFSensorVariableEmitter::newVectorDataReceived(QVector<int> data)
{
	QVariant temp;
	temp.setValue(data);

	emit newDataInFlow(temp, portAssociatedWithProperty["OUT"]);
}
