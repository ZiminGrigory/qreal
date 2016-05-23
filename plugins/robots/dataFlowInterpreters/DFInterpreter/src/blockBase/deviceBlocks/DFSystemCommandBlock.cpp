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

#include "DFSystemCommandBlock.h"

#include <qrutils/stringUtils.h>


///@todo: add "commandAnswer" signal to shell

using namespace dataFlowBlocks::details;

DFSystemCommandBlock::DFSystemCommandBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["COMMAND"] = 2;
}

void DFSystemCommandBlock::init()
{
	mCommandText = stringProperty("Command");
	mIsCode = boolProperty("IsCode");
	mEvaluate = boolProperty("Evaluate");
}

void DFSystemCommandBlock::handleData(Shell &shell)
{
	if (hasNewData("CF_IN")) {
		propertyFromPort("CF_IN");
		execute(shell);
	} else if (hasNewData("COMMAND")) {
		mCommandText = propertyFromPort("COMMAND").toString();
		execute(shell);
	}
}

void DFSystemCommandBlock::execute(Shell &shell)
{
	if (mIsCode) {
		const QString code = utils::StringUtils::dequote(stringProperty("Command").replace("\"", "\\\""));
		shell.runCode(code);
	} else {
		const QString command = utils::StringUtils::wrap(mEvaluate
				? evalCode<QString>(mCommandText)
				: mCommandText.replace("\"", "\\\""));
		shell.runCommand(command);
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
