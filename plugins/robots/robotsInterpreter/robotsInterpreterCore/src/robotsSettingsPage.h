#pragma once

#include <qrgui/dialogs/preferencesPages/preferencesPage.h>

//#include "sensorConstants.h"
//#include "details/sensorsConfigurationWidget.h"
#include "kitPluginManager.h"

namespace Ui {
class PreferencesRobotSettingsPage;
}

namespace robotsInterpreterCore {

/// Preferences page for robots interpreter plugin. Contains constructor kit selector and a placeholder for
/// kit-specific settings.
class RobotsSettingsPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit RobotsSettingsPage(KitPluginManager const &settingsExtensions, QWidget *parent = 0);
	~RobotsSettingsPage();

	virtual void save();
	virtual void restoreSettings();

signals:
	void saved();

protected:
	void changeEvent(QEvent *e);

private:
	QString selectedKit() const;

	Ui::PreferencesRobotSettingsPage *mUi;
	KitPluginManager const &mKitPluginManager;
};

}
