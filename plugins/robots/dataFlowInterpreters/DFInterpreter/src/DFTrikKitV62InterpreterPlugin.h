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

#include <QtCore/QScopedPointer>

#include <kitBase/kitPluginInterface.h>
#include <twoDModel/robotModel/twoDRobotModel.h>
#include <twoDModel/engine/twoDModelControlInterface.h>

#include <plugins/robots/common/trikKit/include/trikKit/robotModel/trikRobotModelBase.h>

#include <trikKitInterpreterCommon/trikAdditionalPreferences.h>
#include <trikKitInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h>

#include "DFFactoryBase.h"

#include "declSpec.h"

namespace trik {
namespace dataFlow{

class ROBOTS_TRIK_KIT_INTERPRETER_DATA_FLOW_EXPORT DFTrikKitV62InterpreterPLugin
		: public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "trik.DFTrikV62KitInterpreterPlugin")

public:
	DFTrikKitV62InterpreterPLugin();
	~DFTrikKitV62InterpreterPLugin();

	void init(const kitBase::KitPluginConfigurator &configurer) override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	kitBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

	QWidget *quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model) override;

	QString defaultSettingsFile() const override;

	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

	kitBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QString kitId() const override;
	QString friendlyKitName() const override;

private slots:
	QWidget *produceIpAddressConfigurer();  // Transfers ownership

private:
	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
	QScopedPointer<trik::robotModel::TrikRobotModelBase> mRealRobotModel;
	QScopedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;

	/// @todo Use shared pointers instead of this sh~.
	/// Ownership depends on mOwnsBlocksFactory flag.
	dataFlowBlocks::DFFactoryBase *mBlocksFactory = nullptr;
	bool mOwnsBlocksFactory = true;

	/// Ownership depends on mOwnsAdditionalPreferences flag.
	TrikAdditionalPreferences *mAdditionalPreferences = nullptr;
	bool mOwnsAdditionalPreferences = true;

	kitBase::InterpreterControlInterface *mInterpreterControl;  // Does not have ownership.
	QString mCurrentlySelectedModelName;

	QAction *mStartInterpretationAction;
};

}
}
