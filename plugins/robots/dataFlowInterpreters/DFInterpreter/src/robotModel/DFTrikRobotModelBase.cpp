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

#include "DFTrikRobotModelBase.h"

#include <kitBase/robotModel/robotParts/display.h>
#include <kitBase/robotModel/robotParts/speaker.h>
#include <kitBase/robotModel/robotParts/button.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include <kitBase/robotModel/robotParts/accelerometerSensor.h>

#include <kitBase/robotModel/robotParts/encoderSensor.h>

#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>

#include "parts/DFTrikServoMotor.h"
#include "parts/DFTrikPowerMotor.h"
#include "parts/DFTrikInfraredSensor.h"
#include "parts/DFTrikSonarSensor.h"
#include "parts/DFTrikMotionSensor.h"
#include "parts/DFTrikColorSensor.h"
#include "parts/DFTrikLineSensor.h"
#include "parts/DFTrikObjectSensor.h"
#include "parts/DFTrikLed.h"
#include "parts/DFTrikShell.h"
#include "parts/DFTrikGamepadButton.h"
#include "parts/DFTrikGamepadConnectionIndicator.h"
#include "parts/DFTrikGamepadPad.h"
#include "parts/DFTrikGamepadPadPressSensor.h"
#include "parts/DFTrikGamepadWheel.h"

using namespace trik::robotModel;
using namespace trik::robotModel::dataFlow;
using namespace kitBase::robotModel;

DFTrikRobotModelBase::DFTrikRobotModelBase(const QString &kitId, const QString &robotId)
	: CommonRobotModel(kitId, robotId)
{
	QList<DeviceInfo> const analogPortConnections = {
		lightSensorInfo()
		, infraredSensorInfo()
		, touchSensorInfo()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });

	addAllowedConnection(PortInfo("Left", input, {}, "buttonLeft"), { buttonInfo() });
	addAllowedConnection(PortInfo("Right", input, {}, "buttonRight"), { buttonInfo() });
	addAllowedConnection(PortInfo("Up", input, {}, "buttonUp"), { buttonInfo() });
	addAllowedConnection(PortInfo("Down", input, {}, "buttonDown"), { buttonInfo() });
	addAllowedConnection(PortInfo("Enter", input, {}, "buttonEnter"), { buttonInfo() });
	addAllowedConnection(PortInfo("Esc", input, {}, "buttonEsc"), { buttonInfo() });

	addAllowedConnection(PortInfo("M1", output, { "М1" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M2", output, { "М2" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M3", output, { "М3" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M4", output, { "М4" }), { powerMotorInfo() });

	addAllowedConnection(PortInfo("A1", input, { "А1" }, "sensorA1"), analogPortConnections);
	addAllowedConnection(PortInfo("A2", input, { "А2" }, "sensorA2"), analogPortConnections);
	addAllowedConnection(PortInfo("A3", input, { "А3" }, "sensorA3"), analogPortConnections);
	addAllowedConnection(PortInfo("A4", input, { "А4" }, "sensorA4"), analogPortConnections);
	addAllowedConnection(PortInfo("A5", input, { "А5" }, "sensorA5"), analogPortConnections);
	addAllowedConnection(PortInfo("A6", input, { "А6" }, "sensorA6"), analogPortConnections);

	addAllowedConnection(PortInfo("D1", input, {}, "sensorD1"), { sonarSensorInfo() });
	addAllowedConnection(PortInfo("D2", input, {}, "sensorD2"), { sonarSensorInfo() });

	addAllowedConnection(PortInfo("GyroscopePort", input, {}, "gyroscope", PortInfo::ReservedVariableType::vector)
			, { gyroscopeInfo() });

	addAllowedConnection(PortInfo("AccelerometerPort", input, {}, "accelerometer"
			, PortInfo::ReservedVariableType::vector), { accelerometerInfo() });

	addAllowedConnection(PortInfo("LedPort", output), { ledInfo() });

	addAllowedConnection(PortInfo("LineSensorPort", tr("Line sensor"), input, { "TrikLineSensorPort" }, "lineSensor"
			, PortInfo::ReservedVariableType::vector), { lineSensorInfo() });

	addAllowedConnection(PortInfo("ObjectSensorXPort", input, {}, "objectSensorX"), { objectSensorInfo() });
	addAllowedConnection(PortInfo("ObjectSensorYPort", input, {}, "objectSensorY"), { objectSensorInfo() });
	addAllowedConnection(PortInfo("ObjectSensorSizePort", input, {}, "objectSensorSize"), { objectSensorInfo() });

	addAllowedConnection(PortInfo("ColorSensorRPort", input, {}, "colorSensorR"), { colorSensorInfo() });
	addAllowedConnection(PortInfo("ColorSensorGPort", input, {}, "colorSensorG"), { colorSensorInfo() });
	addAllowedConnection(PortInfo("ColorSensorBPort", input, {}, "colorSensorB"), { colorSensorInfo() });

	addAllowedConnection(PortInfo("ShellPort", output), { shellInfo() });

	addAllowedConnection(PortInfo("GamepadPad1PosPort", input, {}, "gamepadPad1"
			, PortInfo::ReservedVariableType::vector), { gamepadPadInfo() });
	addAllowedConnection(PortInfo("GamepadPad2PosPort", input, {}, "gamepadPad2"
			, PortInfo::ReservedVariableType::vector), { gamepadPadInfo() });

	addAllowedConnection(PortInfo("GamepadPad1PressedPort", input, {}, "gamepadPad1Pressed")
			, { gamepadPadPressSensorInfo() });
	addAllowedConnection(PortInfo("GamepadPad2PressedPort", input, {}, "gamepadPad2Pressed")
			, { gamepadPadPressSensorInfo() });

	addAllowedConnection(PortInfo("GamepadWheelPort", input, {}, "gamepadWheel"), { gamepadWheelInfo() });

	addAllowedConnection(PortInfo("GamepadButton1Port", input, {}, "gamepadButton1"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton2Port", input, {}, "gamepadButton2"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton3Port", input, {}, "gamepadButton3"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton4Port", input, {}, "gamepadButton4"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton5Port", input, {}, "gamepadButton5"), { gamepadButtonInfo() });

	addAllowedConnection(PortInfo("GamepadConnectionIndicatorPort", input, {}, "gamepadConnected")
			, { gamepadConnectionIndicatorInfo() });
}

QList<PortInfo> DFTrikRobotModelBase::configurablePorts() const
{
	QList<PortInfo> const digitalPorts = {
			  PortInfo("D1", input, {}, "sensorD1")
			, PortInfo("D2", input, {}, "sensorD2")
			};

	return CommonRobotModel::configurablePorts() + digitalPorts + QList<PortInfo>{PortInfo("LineSensorPort"
			, tr("Line sensor"), input, { "TrikLineSensorPort" }, "lineSensor"
			, PortInfo::ReservedVariableType::vector)};
}

QList<DeviceInfo> DFTrikRobotModelBase::convertibleBases() const
{
	return { DeviceInfo::create<robotParts::TouchSensor>()
		, DeviceInfo::create<robotParts::LightSensor>()
		, DeviceInfo::create<parts::TrikInfraredSensor>()
		, DeviceInfo::create<parts::TrikSonarSensor>()
		, DeviceInfo::create<parts::TrikMotionSensor>()
		, DeviceInfo::create<parts::TrikLineSensor>()
	};
}

DeviceInfo DFTrikRobotModelBase::displayInfo() const
{
	return DeviceInfo::create<robotParts::Display>();
}

DeviceInfo DFTrikRobotModelBase::speakerInfo() const
{
	return DeviceInfo::create<robotParts::Speaker>();
}

DeviceInfo DFTrikRobotModelBase::buttonInfo() const
{
	return DeviceInfo::create<robotParts::Button>();
}

DeviceInfo DFTrikRobotModelBase::powerMotorInfo() const
{
	return DeviceInfo::create<robotParts::Motor>();
}

DeviceInfo DFTrikRobotModelBase::servoMotorInfo() const
{
	return DeviceInfo::create<robotParts::Motor>();
}

DeviceInfo DFTrikRobotModelBase::encoderInfo() const
{
	return DeviceInfo::create<robotParts::EncoderSensor>();
}

DeviceInfo DFTrikRobotModelBase::touchSensorInfo() const
{
	return DeviceInfo::create<robotParts::TouchSensor>();
}

DeviceInfo DFTrikRobotModelBase::lightSensorInfo() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}

DeviceInfo DFTrikRobotModelBase::infraredSensorInfo() const
{
	return DeviceInfo::create<parts::TrikInfraredSensor>();
}

DeviceInfo DFTrikRobotModelBase::sonarSensorInfo() const
{
	return DeviceInfo::create<parts::TrikSonarSensor>();
}

DeviceInfo DFTrikRobotModelBase::motionSensorInfo() const
{
	return DeviceInfo::create<parts::TrikMotionSensor>();
}

DeviceInfo DFTrikRobotModelBase::gyroscopeInfo() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>();
}

DeviceInfo DFTrikRobotModelBase::accelerometerInfo() const
{
	return DeviceInfo::create<robotParts::AccelerometerSensor>();
}

DeviceInfo DFTrikRobotModelBase::ledInfo() const
{
	return DeviceInfo::create<parts::TrikLed>();
}

DeviceInfo DFTrikRobotModelBase::lineSensorInfo() const
{
	return DeviceInfo::create<parts::TrikLineSensor>();
}

DeviceInfo DFTrikRobotModelBase::colorSensorInfo() const
{
	return DeviceInfo::create<parts::TrikColorSensor>();
}

DeviceInfo DFTrikRobotModelBase::objectSensorInfo() const
{
	return DeviceInfo::create<parts::TrikObjectSensor>();
}

DeviceInfo DFTrikRobotModelBase::shellInfo() const
{
	return DeviceInfo::create<parts::TrikShell>();
}

DeviceInfo DFTrikRobotModelBase::gamepadButtonInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadButton>();
}

DeviceInfo DFTrikRobotModelBase::gamepadPadInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadPad>();
}

DeviceInfo DFTrikRobotModelBase::gamepadPadPressSensorInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadPadPressSensor>();
}

DeviceInfo DFTrikRobotModelBase::gamepadWheelInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadWheel>();
}

DeviceInfo DFTrikRobotModelBase::gamepadConnectionIndicatorInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadConnectionIndicator>();
}

QHash<QString, int> DFTrikRobotModelBase::buttonCodes() const
{
	QHash<QString, int> result;
	result["LeftButton"] = 105;
	result["RightButton"] = 106;
	result["UpButton"] = 103;
	result["DownButton"] = 108;
	result["EnterButton"] = 28;
	result["PowerButton"] = 116;
	result["EscButton"] = 1;
	return result;
}
