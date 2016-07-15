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

#include "DFSendMessageToRobotBlock.h"

using namespace dataFlowBlocks::details;


DFSendMessageToRobotBlock::DFSendMessageToRobotBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["MSG"] = 2;
	portAssociatedWithProperty["NUMBER"] = 3;
}

void DFSendMessageToRobotBlock::init()
{
	mToRobot = intProperty("robotNum");
	mMsg = stringProperty("message");
}

void DFSendMessageToRobotBlock::handleData(Shell &shell)
{
	if (hasNewData("NUMBER")) {
		mToRobot = propertyFromPort("NUMBER").toInt();
		return;
	}

	if (hasNewData(portAssociatedWithProperty["CF_IN"])) {
		property(portAssociatedWithProperty["CF_IN"]);
	} else if (hasNewData("MSG")) {
		mMsg = propertyFromPort("MSG").toString();
	}

	const QString command = QString("mailbox.send(%1, \"%2\")").arg(QString::number(mToRobot)).arg(mMsg);
	shell.runCommand(command);

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
