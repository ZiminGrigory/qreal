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

#include "DFTrikKitV62InterpreterPlugin.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>

#include <twoDModel/engine/twoDModelEngineFacade.h>
#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>

#include <trikV62KitInterpreter/src/robotModel/real/trikV62RealRobotModel.h>
#include <trikV62KitInterpreter/src/robotModel/twoD/trikV62TwoDRobotModel.h>

using namespace trik;
using namespace trik::dataFlow;
using namespace qReal;

const Id robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
const Id subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");
const QString kitIdString = "trikV62Kit";


DFTrikKitV62InterpreterPLugin::DFTrikKitV62InterpreterPLugin()
{
}

DFTrikKitV62InterpreterPLugin::~DFTrikKitV62InterpreterPLugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}

	if (mOwnsBlocksFactory) {
		delete mBlocksFactory;
	}
}

void DFTrikKitV62InterpreterPLugin::init(const kitBase::KitPluginConfigurator &configurer)
{
	mStartInterpretationAction = new QAction(QIcon(":/icons/dataflow_run.svg")
											 , tr("Run DataFlow Interpretation")
											 , nullptr
	);

	mRealRobotModel.reset(new trik::robotModel::real::RealRobotModel(kitIdString, "trikV62KitRobot"));
	mTwoDRobotModel.reset(new trik::robotModel::twoD::TwoDRobotModel(*(mRealRobotModel.data())));
	mBlocksFactory = new dataFlowBlocks::DFFactoryBase();

	const auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(*mTwoDRobotModel);

	mTwoDRobotModel->setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModel->name() });

	qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			= configurer.qRealConfigurator().mainWindowInterpretersInterface();

	mTwoDModel->init(configurer.eventsForKitPlugin()
			, configurer.qRealConfigurator().systemEvents()
			, configurer.qRealConfigurator().logicalModelApi()
			, interpretersInterface
			, configurer.qRealConfigurator().projectManager()
			, configurer.interpreterControl()
	);

	mRealRobotModel->setErrorReporter(*interpretersInterface.errorReporter());
	mTwoDRobotModel->setErrorReporter(*interpretersInterface.errorReporter());

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, mRealRobotModel.data(), &robotModel::TrikRobotModelBase::rereadSettings);

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, mTwoDRobotModel.data(), &robotModel::twoD::TrikTwoDRobotModel::rereadSettings);
}

QList<kitBase::robotModel::RobotModelInterface *> DFTrikKitV62InterpreterPLugin::robotModels()
{
	return {mRealRobotModel.data(), mTwoDRobotModel.data()};
}

kitBase::blocksBase::BlocksFactoryInterface *DFTrikKitV62InterpreterPLugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model);

	mOwnsBlocksFactory = false;
	return mBlocksFactory;
}

kitBase::robotModel::RobotModelInterface *DFTrikKitV62InterpreterPLugin::defaultRobotModel()
{
	return mTwoDRobotModel.data();
}

QList<kitBase::AdditionalPreferences *> DFTrikKitV62InterpreterPLugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences};
}

QWidget *DFTrikKitV62InterpreterPLugin::quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model)
{
	return model.name().toLower().contains("twod")
			? nullptr
			: produceIpAddressConfigurer();
}

QList<qReal::ActionInfo> DFTrikKitV62InterpreterPLugin::customActions()
{
	return {qReal::ActionInfo(mStartInterpretationAction, "", "tools")};
}

QList<HotKeyActionInfo> DFTrikKitV62InterpreterPLugin::hotKeyActions()
{
	return {};
}

QString DFTrikKitV62InterpreterPLugin::defaultSettingsFile() const
{
	return ":/trikDefaultSettings.ini";
}

QIcon DFTrikKitV62InterpreterPLugin::iconForFastSelector(
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	return &robotModel == mRealRobotModel.data()
			? QIcon(":/icons/switch-real-trik.svg")
			: &robotModel == mTwoDRobotModel.data()
					? QIcon(":/icons/switch-2d.svg")
					: QIcon();
}

kitBase::DevicesConfigurationProvider *DFTrikKitV62InterpreterPLugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

QWidget *DFTrikKitV62InterpreterPLugin::produceIpAddressConfigurer()
{
	QLineEdit * const quickPreferences = new QLineEdit;
	quickPreferences->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	quickPreferences->setPlaceholderText(tr("Enter robot`s IP-address here..."));
	const auto updateQuickPreferences = [quickPreferences]() {
		const QString ip = qReal::SettingsManager::value("TrikTcpServer").toString();
		if (quickPreferences->text() != ip) {
			quickPreferences->setText(ip);
		}
	};

	updateQuickPreferences();
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged, updateQuickPreferences);
	qReal::SettingsListener::listen("TrikTcpServer", updateQuickPreferences, this);
	connect(quickPreferences, &QLineEdit::textChanged, [](const QString &text) {
		qReal::SettingsManager::setValue("TrikTcpServer", text);
	});

	return quickPreferences;
}

QString DFTrikKitV62InterpreterPLugin::kitId() const
{
	return kitIdString;
}

QString DFTrikKitV62InterpreterPLugin::friendlyKitName() const
{
	return tr("TRIK (model-2015)");
}
