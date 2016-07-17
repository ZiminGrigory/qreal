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

QT += widgets network

includes(plugins/robots/common/kitBase \
		qrtext \
		plugins/robots/interpreters/nxtKitInterpreter/src \
		plugins/robots/common/twoDModel \
		plugins/robots/common/nxtKit \
		plugins/robots/utils \
		plugins/robots/interpreters/ \
		plugins/robots/interpreters/interpreterCore \
		plugins/robots/dataFlowInterpreters \
		plugins/robots/dataFlowInterpreters\DFInterpreter \
		)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model robots-nxt-kit robots-nxt-interpreter qextserialport qslog dataflow-interpreter \
		)

HEADERS += \
	$$PWD/DFNxtKitInterpreterPlugin.h \

SOURCES += \
	$$PWD/DFNxtKitInterpreterPlugin.cpp \

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/DFNxtKitInterpreter_ru.ts \

