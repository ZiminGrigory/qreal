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
#pragma once

#include "plugins/robots/dataFlowInterpreters/DFInterpreter/src/blockBase/DFDeviceBlock.h"

#include <trikKit/robotModel/parts/trikShell.h>

namespace dataFlowBlocks {
namespace details {

using Shell = trik::robotModel::parts::TrikShell;
using ShellDevice = DFDeviceBlock<Shell>;

class DFReadFileBlock : public ShellDevice
{
	Q_OBJECT

public:
	explicit DFReadFileBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	void init() override;
	void handleData(Shell &shell) override;

private slots:
	void handleText(QString text);

private:
	QString mFileName = QString();
	bool mIsGenerateByWords = true;
};

}
}
