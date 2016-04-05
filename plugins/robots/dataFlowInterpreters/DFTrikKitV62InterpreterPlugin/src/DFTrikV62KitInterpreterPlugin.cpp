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

#include "DFTrikV62KitInterpreterPlugin.h"
#include <DFInterpreter/src/DFFactoryBase.h>


using namespace trik;
using namespace qReal;

const Id robotDiagramType = Id("RobotsDataFlowMetamodel", "RobotsDataFlowDiagram", "RobotsBehaviourDiagram");
const Id subprogramDiagramType = Id("RobotsDataFlowMetamodel", "RobotsDataFlowDiagram", "RobotsBehaviourSubprogram");
const QString kitIdString = "trikV62Kit";


DFTrikV62KitInterpreterPlugin::DFTrikV62KitInterpreterPlugin()
	: TrikKitInterpreterPluginBase()
{
}

QString DFTrikV62KitInterpreterPlugin::kitId() const
{
	return kitIdString;
}

void DFTrikV62KitInterpreterPlugin::init(const kitBase::KitPluginConfigurator &configurer)
{
	//TrikKitInterpreterPluginBase::init(configurer);

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

//	configurer.robotModelManager().model()

//	mBlocksFactoryManager.addFactory(mBlocksFactory, mRealModel);
//	mBlocksFactoryManager.addFactory(mBlocksFactory, mTwoDModel);
}

kitBase::blocksBase::BlocksFactoryInterface *DFTrikV62KitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model);
	return {};
}

QString DFTrikV62KitInterpreterPlugin::friendlyKitName() const
{
	// already defined
	return QString();
}

QList<kitBase::robotModel::RobotModelInterface *> DFTrikV62KitInterpreterPlugin::robotModels()
{
	// already defined
	return {};
}

kitBase::DevicesConfigurationProvider *DFTrikV62KitInterpreterPlugin::devicesConfigurationProvider()
{
	return {};
}


QList<kitBase::InterpreterInterface *> DFTrikV62KitInterpreterPlugin::customInterpreters() const
{
	return {mInterpreter};
}
