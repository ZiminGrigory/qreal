#include "DFConcreteWritablePort.h"

#include <QApplication>
#include <QtCore/QSet>

#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/motor.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>

using namespace dataFlowBlocks::details;
using namespace kitBase::robotModel;

const QSet<QString> scalarSensors = {"A6", "A5", "A4", "A3", "A2", "A1", "GamepadButton1", "GamepadButton2"
		, "GamepadButton3", "GamepadButton4", "GamepadButton5", "Down", "Enter", "Esc", "Left"
		, "Right", "Up"};

const QSet<QString> motors = {"M1", "M2", "M3", "M4"};

const QSet<QString> encoders = {"E1", "E2", "E3", "E4"};


DFConcreteWritablePort::DFConcreteWritablePort(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["DATA"] = 2;
}

void DFConcreteWritablePort::handleData()
{
	auto handle = [&](int impulse){
		if (scalarSensors.contains(mSelectedPortName)) {
			PortInfo const portInfo = RobotModelUtils::findPort(mRobotModel, mSelectedPortName, input);
			auto scalarSensor = RobotModelUtils::findDevice<robotParts::ScalarSensor>(mRobotModel, portInfo);
			scalarSensor->setLastData(impulse);
		} else if (motors.contains(mSelectedPortName)) {
			PortInfo const portInfo = RobotModelUtils::findPort(mRobotModel, mSelectedPortName, output);
			auto motorDevice = RobotModelUtils::findDevice<robotParts::Motor>(mRobotModel, portInfo);
			motorDevice->on(impulse);
		} else if (encoders.contains(mSelectedPortName)) {
			auto encoder = RobotModelUtils::findDevice<robotParts::EncoderSensor>(mRobotModel, mSelectedPortName);
			encoder->nullify();
		}
	};

	if (hasNewData(portAssociatedWithProperty["CF_IN"])) {
		property(portAssociatedWithProperty["CF_IN"]);
		handle(mImpulse);
	} else if (hasNewData(portAssociatedWithProperty["DATA"])) {
		handle(propertyFromPort("DATA").toInt());
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}

void DFConcreteWritablePort::init()
{
	mSelectedPortName = stringProperty("portName");
	mImpulse = intProperty("portmImpulse");
}
