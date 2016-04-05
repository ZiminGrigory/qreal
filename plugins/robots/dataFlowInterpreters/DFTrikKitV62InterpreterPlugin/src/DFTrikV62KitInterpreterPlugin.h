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

#include <trikKitInterpreterCommon/trikKitInterpreterPluginBase.h>
#include <DFInterpreter/src/DFInterpreter.h>
#include <DFInterpreter/src/DFRobotBlocksFactoryInterface.h>

namespace trik {

class DFTrikV62KitInterpreterPlugin : public TrikKitInterpreterPluginBase
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "trik.TrikV62KitInterpreterPlugin")

public:
	DFTrikV62KitInterpreterPlugin();

	void init(const kitBase::KitPluginConfigurator &configurer) override;
	QString kitId() const override;
	QList<kitBase::InterpreterInterface *> customInterpreters() const override;
	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model);
	QString friendlyKitName() const override;
	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;
private:
	interpreterCore::interpreter::dataFlowInterpretation::DFInterpeter *mInterpreter; // has ownership
	dataFlow::blocksBase::DFRobotBlocksFactoryInterface *mFactory;
};

}

