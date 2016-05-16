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

#include "DFRemoveFileBlock.h"

using namespace dataFlowBlocks::details;

DFRemoveFileBlock::DFRemoveFileBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["CF_IN"] = 1;
	portAssociatedWithProperty["FILE"] = 2;
}

void DFRemoveFileBlock::init()
{
	mFileName = stringProperty("fileName");
}

void DFRemoveFileBlock::handleData(Shell &shell)
{
	if (hasNewData("CF_IN")) {
		shell.removeFile(mFileName);
	} else if (hasNewData("FILE")) {
		mFileName = propertyFromPort("FILE").toString();
		shell.removeFile(mFileName);
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
