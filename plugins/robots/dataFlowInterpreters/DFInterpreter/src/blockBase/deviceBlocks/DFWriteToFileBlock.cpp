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

#include "DFWriteToFileBlock.h"

using namespace dataFlowBlocks::details;

DFWriteToFileBlock::DFWriteToFileBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["CF_IN"] = 1;
	portAssociatedWithProperty["FILE"] = 2;
	portAssociatedWithProperty["TEXT"] = 3;
}

void DFWriteToFileBlock::init()
{
	mFileName = stringProperty("fileName");
	mText = stringProperty("text");
}

void DFWriteToFileBlock::handleData(Shell &shell)
{
	if (hasNewData("CF_IN")) {
		shell.writeToFile(mFileName, mText);
	} else if (hasNewData("FILE")) {
		mFileName = propertyFromPort("FILE").toString();
	} else if (hasNewData("TEXT")) {
		mText = propertyFromPort("TEXT").toString();
		shell.writeToFile(mFileName, mText);
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
