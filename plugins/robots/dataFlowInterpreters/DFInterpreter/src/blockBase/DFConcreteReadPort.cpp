#include "DFConcreteReadPort.h"

#include <QApplication>
#include <QtCore/QSet>

#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/motor.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>

using namespace dataFlowBlocks::details;
using namespace kitBase::robotModel;


const QSet<QString> scalarSensors = {"A6", "A5", "A4", "A3", "A2", "A1"
		, "GamepadButton1", "GamepadButton2", "GamepadButton3", "GamepadButton4", "GamepadButton5"
		, "D1", "D2"
		, "E1", "E2", "E3", "E4"
		, "GamepadPad1PosPort", "GamepadPad2PressedPort", "GamepadConnectionIndicatorPort"
		, "Down", "Enter", "Esc", "Left", "Right", "Up"};

const QSet<QString> vectorSensors = {"GyroscopePort", "AccelerometerPort", "LineSensorPort", "ObjectSensorXPort"
		, "ObjectSensorYPort", "ObjectSensorSizePort", "ColorSensorRPort", "ColorSensorGPort", "ColorSensorBPort"
		, "GamepadPad1PosPort", "GamepadPad2PosPort"};

const QSet<QString> motors = {"M1", "M2", "M3", "M4"};


DFConcreteReadPort::DFConcreteReadPort(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["OUT"] = 1;
}

void DFConcreteReadPort::handleData()
{

	if (scalarSensors.contains(mSelectedPortName)) {
		PortInfo const portInfo = RobotModelUtils::findPort(mRobotModel, mSelectedPortName, input);
		auto scalarSensor = RobotModelUtils::findDevice<robotParts::ScalarSensor>(mRobotModel, portInfo);
		emit newDataInFlow(scalarSensor->lastData(), portAssociatedWithProperty["OUT"]);
	} else if (vectorSensors.contains(mSelectedPortName)) {
		PortInfo const portInfo = RobotModelUtils::findPort(mRobotModel, mSelectedPortName, input);
		auto vectorSensor = RobotModelUtils::findDevice<robotParts::VectorSensor>(mRobotModel, portInfo);
		QVariantList outputData = vectorToQVariantList(vectorSensor->lastData());
		emit newDataInFlow(QVariant(outputData), portAssociatedWithProperty["OUT"]);
	} else if (motors.contains(mSelectedPortName)) {
		PortInfo const portInfo = RobotModelUtils::findPort(mRobotModel, mSelectedPortName, output);
		auto motorDevice = RobotModelUtils::findDevice<robotParts::Motor>(mRobotModel, portInfo);
		emit newDataInFlow(QVariant(motorDevice->power()), portAssociatedWithProperty["OUT"]);
	}
}

void DFConcreteReadPort::init()
{
	mSelectedPortName = stringProperty("portName");
}
