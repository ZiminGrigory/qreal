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
		plugins/robots/common/twoDModel \
		plugins/robots/common/ev3Kit \
		plugins/robots/interpreters/interpreterCore \
		plugins/robots/utils \
		qrtext \
		plugins/robots/interpeters/interpreterCore/src \
		plugins/robots/thirdparty/libusb-1.0.19/include/libusb-1.0 \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-ev3-kit robots-2d-model qextserialport qslog \
		robots-interpreter-core \
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
	$$PWD/cosmoThread.h \
	$$PWD/cosmoInterpreter.h \
    $$PWD/dataFlowRobotsBlock.h

SOURCES += \
	$$PWD/cosmoThread.cpp \
	$$PWD/cosmoInterpreter.cpp \
    $$PWD/dataFlowRobotsBlock.cpp
