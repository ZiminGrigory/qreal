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

#include "DFSayBlock.h"

using namespace dataFlowBlocks::details;

const int activationPort = -1;

DFSayBlock::DFSayBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["DATA"] = 1;
}

void DFSayBlock::init()
{
	mIsEvaluate = boolProperty("Evaluate");
}

int DFSayBlock::activationPortNumber() const
{
	return activationPort;
}

void DFSayBlock::handleData(Shell &shell)
{
	if (hasNewData(activationPort)) {
		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
		return;
	}

	QString rawText = propertyFromPort("DATA").toString();
	const QString text = mIsEvaluate ? evalCode<QString>(rawText) : rawText;
	if (!errorsOccured()) {
		shell.say(text);
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
