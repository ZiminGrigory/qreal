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
		plugins/robots/dataFlowInterpreters \
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
    $$PWD/blockBase/DFActiveUniqueDeviceBlock.h \
    $$PWD/blockBase/DFDeviceBlock.h \
    $$PWD/blockBase/DFMotorsBlock.h \
    $$PWD/blockBase/DFValueEmitter.h \
    $$PWD/DFInterpreter.h \
    $$PWD/blockBase/DFFunctionBlock.h \
    $$PWD/blockBase/DFSensorVariableEmitter.h \
    $$PWD/DFRobotsBlockInterface.h \
    $$PWD/DFRobotBlocksFactoryInterface.h \
    $$PWD/DFRobotBlocksTableInterface.h \
    $$PWD/DFRobotBlocksTableBase.h \
    $$PWD/DFRobotBlocksTable.h \
    $$PWD/blockBase/DFInhibitionBlock.h \
    $$PWD/blockBase/DFSupressorBlock.h \
    $$PWD/blockBase/DFInterflowingBlock.h \
    $$PWD/blockBase/DFExitExecutionBlock.h \
    $$PWD/blockBase/DFDelayAndConditionFilter.h \
    $$PWD/blockBase/DFInputSubProgramPort.h \
    $$PWD/blockBase/DFOutputSubProgramPort.h \
    $$PWD/blockBase/DFGamepadBlock.h \
    $$PWD/blockBase/DFSubprogramCall.h \
    $$PWD/blockBase/DFVariable.h \
    $$PWD/blockBase/DFWaitNode.h \
    $$PWD/blockBase/DFRandomValue.h \
    $$PWD/blockBase/DFConditionBlock.h \
    $$PWD/blockBase/DFLoopBlock.h \
    $$PWD/blockBase/DFSwitchBlock.h \
    $$PWD/blockBase/DFSeparatorBlock.h \
    $$PWD/blockBase/DFServoMotorsBlock.h \
    $$PWD/blockBase/deviceBlocks/DFSmileBlock.h \
    $$PWD/blockBase/deviceBlocks/DFClearScreenBlock.h \
    $$PWD/blockBase/deviceBlocks/DFSetPenBlock.h \
    $$PWD/blockBase/deviceBlocks/DFPaintSettingsBlock.h

SOURCES += \
    $$PWD/DFFactoryBase.cpp \
    $$PWD/DFRobotsBlock.cpp \
    $$PWD/DFThread.cpp \
    $$PWD/blockBase/DFMotorsBlock.cpp \
    $$PWD/blockBase/DFValueEmitter.cpp \
    $$PWD/DFInterpreter.cpp \
    $$PWD/blockBase/DFFunctionBlock.cpp \
    $$PWD/blockBase/DFSensorVariableEmitter.cpp \
    $$PWD/DFRobotBlocksTableBase.cpp \
    $$PWD/DFRobotBlocksTable.cpp \
    $$PWD/blockBase/DFInhibitionBlock.cpp \
    $$PWD/blockBase/DFSupressorBlock.cpp \
    $$PWD/blockBase/DFInterflowingBlock.cpp \
    $$PWD/blockBase/DFExitExecutionBlock.cpp \
    $$PWD/blockBase/DFDelayAndConditionFilter.cpp \
    $$PWD/blockBase/DFInputSubProgramPort.cpp \
    $$PWD/blockBase/DFOutputSubProgramPort.cpp \
    $$PWD/blockBase/DFGamepadBlock.cpp \
    $$PWD/blockBase/DFSubprogramCall.cpp \
    $$PWD/blockBase/DFVariable.cpp \
    $$PWD/blockBase/DFWaitNode.cpp \
    $$PWD/blockBase/DFRandomValue.cpp \
    $$PWD/blockBase/DFConditionBlock.cpp \
    $$PWD/blockBase/DFLoopBlock.cpp \
    $$PWD/blockBase/DFSwitchBlock.cpp \
    $$PWD/blockBase/DFSeparatorBlock.cpp \
    $$PWD/blockBase/DFServoMotorsBlock.cpp \
    $$PWD/blockBase/deviceBlocks/DFSmileBlock.cpp \
    $$PWD/blockBase/deviceBlocks/DFClearScreenBlock.cpp \
    $$PWD/blockBase/deviceBlocks/DFSetPenBlock.cpp \
    $$PWD/blockBase/deviceBlocks/DFPaintSettingsBlock.cpp
