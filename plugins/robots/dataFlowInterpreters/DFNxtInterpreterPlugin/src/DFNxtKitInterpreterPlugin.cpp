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

#include "DFNxtKitInterpreterPlugin.h"
#include <DFInterpreter/src/DFFactoryBase.h>


using namespace nxt;
using namespace qReal;

const Id robotDiagramType = Id("RobotsDataFlowMetamodel", "RobotsDataFlowDiagram", "RobotsBehaviourDiagram");
const Id subprogramDiagramType = Id("RobotsDataFlowMetamodel", "RobotsDataFlowDiagram", "RobotsBehaviourSubprogram");


DFNxtKitInterpreterPlugin::DFNxtKitInterpreterPlugin()
{
}

void DFNxtKitInterpreterPlugin::init(const kitBase::KitPluginConfigurator &configurer)
{
	NxtKitInterpreterPlugin::init(configurer);

	mFactory = new dataFlow::blocksBase::DFFactoryBase();
	mFactory->configure(configurer.qRealConfigurator().graphicalModelApi()
			, configurer.qRealConfigurator().logicalModelApi()
			, configurer.robotModelManager()
			, *(configurer.qRealConfigurator().mainWindowInterpretersInterface().errorReporter())
			, configurer.textLanguage()
			);

	mInterpreter = new interpreterCore::interpreter::dataFlowInterpretation::DFInterpeter(
			configurer.qRealConfigurator().graphicalModelApi()
			, configurer.qRealConfigurator().logicalModelApi()
			, configurer.qRealConfigurator().mainWindowInterpretersInterface()
			, configurer.qRealConfigurator().projectManager()
			, *mFactory
			, configurer.robotModelManager()
			, configurer.textLanguage()
	);
}

kitBase::blocksBase::BlocksFactoryInterface *DFNxtKitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model);
	return {};
}


QList<kitBase::InterpreterInterface *> DFNxtKitInterpreterPlugin::customInterpreters() const
{
	return {mInterpreter};
}
