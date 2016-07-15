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

DFSayBlock::DFSayBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["DATA"] = 2;
}

void DFSayBlock::init()
{
	mIsEvaluate = boolProperty("Evaluate");
	mText = stringProperty("text");
}

void DFSayBlock::handleData(Shell &shell)
{
	if (hasNewData(portAssociatedWithProperty["CF_IN"])) {
		emit newDataInFlow(property(portAssociatedWithProperty["CF_IN"]), portAssociatedWithProperty["CF_OUT"]);
	}

	if (hasNewData(portAssociatedWithProperty["DATA"])) {
		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
		mText = propertyFromPort("DATA").toString();
	}

	const QString text = mIsEvaluate ? evalCode<QString>(mText) : mText;

	if (!errorsOccured()) {
		shell.say(text);
	}

}
