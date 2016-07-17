/* Copyright 2007-2016 QReal Research Group
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

#include <nxtKitInterpreter/src/nxtKitInterpreterPlugin.h>
#include <DFInterpreter/src/DFInterpreter.h>
#include <DFInterpreter/src/DFRobotBlocksFactoryInterface.h>

namespace nxt {

class DFNxtKitInterpreterPlugin : public NxtKitInterpreterPlugin
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxt.NxtKitInterpreterPlugin")

public:
	DFNxtKitInterpreterPlugin();

	void init(const kitBase::KitPluginConfigurator &configurer) override;
	QList<kitBase::InterpreterInterface *> customInterpreters() const override;
	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model);
private:
	interpreterCore::interpreter::dataFlowInterpretation::DFInterpeter *mInterpreter; // has ownership
	dataFlow::blocksBase::DFRobotBlocksFactoryInterface *mFactory;
};

}
