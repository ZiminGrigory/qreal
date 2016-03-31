# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

includes(plugins/robots/common/kitBase \
		plugins/robots/common/ \
		plugins/robots/common/twoDModel \
		plugins/robots/common/trikKit \
		plugins/robots/common/trikKit/include \
		plugins/robots/interpreters/interpreterCore \
		plugins/robots/utils \
		plugins/robots/interpreters \
		plugins/robots/interpreters/trikKitInterpreterCommon \
		plugins/robots/interpreters/trikV62KitInterpreter \
		plugins/robots/interpreters/trikKitInterpreterCommon/include \
		plugins/robots/interpreters/trikKitInterpreterCommon/include \
		qrtext \
		plugins/robots/interpeters/interpreterCore/src \
		plugins/robots/thirdparty/libusb-1.0.19/include/libusb-1.0 \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-ev3-kit robots-2d-model qextserialport qslog \
		robots-interpreter-core robots-trik-kit-interpreter-common \
)

win32 {
	LIBS += -llibusb-1.0
}

unix:!macx {
	LIBS += -L /usr/bin -lusb-1.0
}

macx {
	LIBS += -lusb-1.0
}

HEADERS += \
    $$PWD/declSpec.h \
    $$PWD/DFFactoryBase.h \
    $$PWD/DFRobotsBlock.h \
    $$PWD/DFThread.h \
    $$PWD/DFTrikKitV62InterpreterPlugin.h \
    $$PWD/blockBase/DFActiveUniqueDeviceBlock.h \
    $$PWD/blockBase/DFDeviceBlock.h \
    $$PWD/blockBase/DFMotorsBlock.h \
    $$PWD/blockBase/DFValueEmitter.h \
    $$PWD/robotModel/DFTrikRobotModelBase.h \
    $$PWD/robotModel/parts/DFTrikColorSensor.h \
    $$PWD/robotModel/parts/DFTrikDisplay.h \
    $$PWD/robotModel/parts/DFTrikGamepadButton.h \
    $$PWD/robotModel/parts/DFTrikGamepadConnectionIndicator.h \
    $$PWD/robotModel/parts/DFTrikGamepadPad.h \
    $$PWD/robotModel/parts/DFTrikGamepadPadPressSensor.h \
    $$PWD/robotModel/parts/DFTrikGamepadWheel.h \
    $$PWD/robotModel/parts/DFTrikInfraredSensor.h \
    $$PWD/robotModel/parts/DFTrikLed.h \
    $$PWD/robotModel/parts/DFTrikLineSensor.h \
    $$PWD/robotModel/parts/DFTrikMotionSensor.h \
    $$PWD/robotModel/parts/DFTrikObjectSensor.h \
    $$PWD/robotModel/parts/DFTrikServoMotor.h \
    $$PWD/robotModel/parts/DFTrikShell.h \
    $$PWD/robotModel/parts/DFTrikSonarSensor.h \
    $$PWD/robotModel/parts/DFTrikSpeaker.h \
    $$PWD/robotModel/parts/DFTrikPowerMotor.h \
    $$PWD/robotModel/DFTrikRobotModelV62.h \
    $$PWD/robotModel/DFTwoDRobotModelV62.h \
    $$PWD/robotModel/DFRealRobotModelV62.h \
    $$PWD/DFInterpreter.h

SOURCES += \
    $$PWD/DFFactoryBase.cpp \
    $$PWD/DFRobotsBlock.cpp \
    $$PWD/DFThread.cpp \
    $$PWD/DFTrikKitV62InterpreterPlugin.cpp \
    $$PWD/blockBase/DFMotorsBlock.cpp \
    $$PWD/blockBase/DFValueEmitter.cpp \
    $$PWD/robotModel/DFTrikRobotModelBase.cpp \
    $$PWD/robotModel/parts/DFTrikColorSensor.cpp \
    $$PWD/robotModel/parts/DFTrikDisplay.cpp \
    $$PWD/robotModel/parts/DFTrikGamepadButton.cpp \
    $$PWD/robotModel/parts/DFTrikGamepadConnectionIndicator.cpp \
    $$PWD/robotModel/parts/DFTrikGamepadPad.cpp \
    $$PWD/robotModel/parts/DFTrikGamepadPadPressSensor.cpp \
    $$PWD/robotModel/parts/DFTrikGamepadWheel.cpp \
    $$PWD/robotModel/parts/DFTrikInfraredSensor.cpp \
    $$PWD/robotModel/parts/DFTrikLed.cpp \
    $$PWD/robotModel/parts/DFTrikLineSensor.cpp \
    $$PWD/robotModel/parts/DFTrikMotionSensor.cpp \
    $$PWD/robotModel/parts/DFTrikObjectSensor.cpp \
    $$PWD/robotModel/parts/DFTrikPowerMotor.cpp \
    $$PWD/robotModel/parts/DFTrikServoMotor.cpp \
    $$PWD/robotModel/parts/DFTrikShell.cpp \
    $$PWD/robotModel/parts/DFTrikSonarSensor.cpp \
    $$PWD/robotModel/parts/DFTrikSpeaker.cpp \
    $$PWD/robotModel/DFTrikRobotModelV62.cpp \
    $$PWD/robotModel/DFTwoDRobotModelV62.cpp \
    $$PWD/robotModel/DFRealRobotModelV62.cpp \
    $$PWD/DFInterpreter.cpp
