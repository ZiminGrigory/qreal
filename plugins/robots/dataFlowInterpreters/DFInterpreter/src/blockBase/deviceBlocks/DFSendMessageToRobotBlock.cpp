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

const int dummyActivationPortNumber = -1;

DFSendMessageToRobotBlock::DFSendMessageToRobotBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["MSG"] = 1;
	portAssociatedWithProperty["NUMBER"] = 2;
}

int DFSendMessageToRobotBlock::activationPortNumber() const
{
	return dummyActivationPortNumber;
}

void DFSendMessageToRobotBlock::init()
{
	mToRobot = intProperty("robotNum");
}

void DFSendMessageToRobotBlock::handleData(Shell &shell)
{
	if (hasNewData(dummyActivationPortNumber)) {
		property(dummyActivationPortNumber);
	} else if (hasNewData("NUMBER")) {
		mToRobot = propertyFromPort("NUMBER").toInt();
	} else if (hasNewData("MSG")) {
		QString msg = propertyFromPort("MSG").toString();
		const QString command = QString("mailbox.send(\"%1\", %2)").arg(QString(mToRobot)).arg(msg);
		shell.runCommand(command);
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
