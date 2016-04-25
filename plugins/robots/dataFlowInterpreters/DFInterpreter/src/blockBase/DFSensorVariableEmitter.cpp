#include "DFSensorVariableEmitter.h"

#include <QApplication>

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
		disconnect(mScalarSensor, &robotParts::ScalarSensor::newData
				, this, &DFSensorVariableEmitter::newIntDataReceived);
		connect(mScalarSensor, &robotParts::ScalarSensor::newData
				, this, &DFSensorVariableEmitter::newIntDataReceived, Qt::QueuedConnection);
	}

	if (mVectorSensor) {
		disconnect(mVectorSensor, &robotParts::VectorSensor::newData
				, this, &DFSensorVariableEmitter::newVectorDataReceived);
		connect(mVectorSensor, &robotParts::VectorSensor::newData
				, this, &DFSensorVariableEmitter::newVectorDataReceived, Qt::QueuedConnection);
	}
}

void DFSensorVariableEmitter::init()
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
	qDebug() << Q_FUNC_INFO;
	QCoreApplication::processEvents();
}

void DFSensorVariableEmitter::newVectorDataReceived(QVector<int> data)
{
	QVariant temp;
	temp.setValue(data);

	emit newDataInFlow(temp, portAssociatedWithProperty["OUT"]);

	QCoreApplication::processEvents();
}
