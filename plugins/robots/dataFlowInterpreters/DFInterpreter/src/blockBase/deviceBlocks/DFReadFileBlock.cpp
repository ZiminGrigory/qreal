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

#include "DFReadFileBlock.h"

#include <QtWidgets/QApplication>

using namespace dataFlowBlocks::details;

DFReadFileBlock::DFReadFileBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["FILE"] = 2;
	portAssociatedWithProperty["OUT"] = 3;
}

void DFReadFileBlock::init()
{
	mFileName = stringProperty("fileName");
	mIsGenerateByWords = boolProperty("isbyWords");
}

void DFReadFileBlock::handleData(Shell &shell)
{
	disconnect(&shell, &Shell::fileContents, this, &DFReadFileBlock::handleText);
	connect(&shell, &Shell::fileContents, this, &DFReadFileBlock::handleText, Qt::QueuedConnection);

	if (hasNewData("CF_IN")) {
		shell.readFile(mFileName);
	} else if (hasNewData("FILE")) {
		mFileName = propertyFromPort("FILE").toString();
		shell.readFile(mFileName);
	}
}

void DFReadFileBlock::handleText(QString text)
{
	QStringList textToGenerate;
	if (mIsGenerateByWords) {
		textToGenerate = text.split(" ");
	} else {
		textToGenerate = text.split("\n");
	}

	for (const QString &str : textToGenerate) {
		emit newDataInFlow(str, portAssociatedWithProperty["OUT"]);
		QApplication::processEvents();
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
