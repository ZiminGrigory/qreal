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

#include "DFRealRobotModelV62.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include <trikKitInterpreterCommon/robotModel/real/parts/display.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/speaker.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/button.h>

#include <trikKitInterpreterCommon/robotModel/real/parts/powerMotor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/servoMotor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/encoderSensor.h>

#include <trikKitInterpreterCommon/robotModel/real/parts/touchSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/lightSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/infraredSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/sonarSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/motionSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/gyroscope.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/accelerometer.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/colorSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/led.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/lineSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/objectSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/shell.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/gamepadButton.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/gamepadConnectionIndicator.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/gamepadPad.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/gamepadPadPressSensor.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/gamepadWheel.h>

#include <utils/robotCommunication/networkCommunicationErrorReporter.h>


using namespace trik::robotModel::real;
using namespace trik::robotModel::dataFlow::real;
using namespace kitBase::robotModel;

DFRealRobotModel::DFRealRobotModel(const QString &kitId, const QString &robotId)
	: DFTrikRobotModelV62(kitId, robotId)
	, mRobotCommunicator(new utils::robotCommunication::TcpRobotCommunicator("TrikTcpServer"))
{
	connect(mRobotCommunicator.data(), &utils::robotCommunication::TcpRobotCommunicator::connected
			, this, &DFRealRobotModel::connected);
	connect(mRobotCommunicator.data(), &utils::robotCommunication::TcpRobotCommunicator::disconnected
			, this, &DFRealRobotModel::disconnected);
}

QString DFRealRobotModel::name() const
{
	return "TrikV62DFRealRobotModel";
}

QString DFRealRobotModel::friendlyName() const
{
	return tr("Interpretation (Wi-Fi)");
}

int DFRealRobotModel::priority() const
{
	return 8;  // Right after qts generator
}

bool DFRealRobotModel::needsConnection() const
{
	return true;
}

void DFRealRobotModel::connectToRobot()
{
	mRobotCommunicator->connect();
}

void DFRealRobotModel::stopRobot()
{
	mRobotCommunicator->stopRobot();
}

void DFRealRobotModel::disconnectFromRobot()
{
	mRobotCommunicator->disconnect();
}

void DFRealRobotModel::setErrorReporter(qReal::ErrorReporterInterface &errorReporter)
{
	utils::robotCommunication::NetworkCommunicationErrorReporter::connectErrorReporter(
			*mRobotCommunicator
			, errorReporter
			);
}

robotParts::Device *DFRealRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
{
	if (deviceInfo.isA(displayInfo())) {
		return new robotModel::real::parts::Display(displayInfo(), port, *mRobotCommunicator);
	}  else if (deviceInfo.isA(speakerInfo())) {
		return new robotModel::real::parts::Speaker(speakerInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(gamepadButtonInfo())) {
		return new robotModel::real::parts::GamepadButton(gamepadButtonInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(gamepadPadPressSensorInfo())) {
		return new robotModel::real::parts::GamepadPadPressSensor(gamepadPadPressSensorInfo()
																  , port
																  , *mRobotCommunicator
		);

	} else if (deviceInfo.isA(buttonInfo())) {
		return new robotModel::real::parts::Button(buttonInfo()
												   , port
												   , buttonCodes()[port.name() + "Button"]
												   , *mRobotCommunicator
		);

	} else if (deviceInfo.isA(powerMotorInfo())) {
		return new robotModel::real::parts::PowerMotor(powerMotorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(servoMotorInfo())) {
		return new robotModel::real::parts::ServoMotor(servoMotorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(encoderInfo())) {
		return new robotModel::real::parts::EncoderSensor(encoderInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(touchSensorInfo())) {
		return new robotModel::real::parts::TouchSensor(touchSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(lightSensorInfo())) {
		return new robotModel::real::parts::LightSensor(lightSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(infraredSensorInfo())) {
		return new robotModel::real::parts::InfraredSensor(infraredSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(sonarSensorInfo())) {
		return new robotModel::real::parts::SonarSensor(sonarSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(motionSensorInfo())) {
		return new robotModel::real::parts::MotionSensor(motionSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(gyroscopeInfo())) {
		return new robotModel::real::parts::Gyroscope(motionSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(accelerometerInfo())) {
		return new robotModel::real::parts::Accelerometer(motionSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(lineSensorInfo())) {
		return new robotModel::real::parts::LineSensor(lineSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(colorSensorInfo())) {
		return new robotModel::real::parts::ColorSensor(colorSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(objectSensorInfo())) {
		return new robotModel::real::parts::ObjectSensor(objectSensorInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(ledInfo())) {
		return new robotModel::real::parts::Led(ledInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(shellInfo())) {
		return new robotModel::real::parts::Shell(shellInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(gamepadConnectionIndicatorInfo())) {
		return new robotModel::real::parts::GamepadConnectionIndicator(gamepadConnectionIndicatorInfo()
																	   , port
																	   , *mRobotCommunicator
		);

	} else if (deviceInfo.isA(gamepadPadInfo())) {
		return new robotModel::real::parts::GamepadPad(gamepadPadInfo(), port, *mRobotCommunicator);
	} else if (deviceInfo.isA(gamepadWheelInfo())) {
		return new robotModel::real::parts::GamepadWheel(gamepadWheelInfo(), port, *mRobotCommunicator);
	}

	return DFTrikRobotModelBase::createDevice(port, deviceInfo);
}
