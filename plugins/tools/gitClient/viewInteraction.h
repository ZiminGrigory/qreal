#pragma once

#include <QtCore/QObject>

#include <qrgui/plugins/toolPluginInterface/pluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/tabInfo.h>

#include "gui/versioningPage.h"
#include "transparentMode.h"

namespace git {

class GitPlugin;

namespace details {

using qReal::TabInfo;

class ViewInteraction : public QObject
{
	Q_OBJECT

public:
	explicit ViewInteraction(GitPlugin *pluginInstance);

	QList<qReal::ActionInfo> actions() const;
	QPair<QString, qReal::gui::PreferencesPage *> preferencesPage() const;
	void init(const qReal::PluginConfigurator &configurator);
	QWidget *makeDiffTab(const QString &name);
	void reopenWithoutSavings();
	QString currentProject();

public slots:
	void initClicked();
	void cloneClicked();
	void commitClicked();
	void remoteClicked();
	void pushClicked();
	void pullClicked();
	void resetClicked();
	void cleanClicked();
	void statusClicked();
	void logClicked();
	void remoteListClicked();
	void versionsClicked();
	void diffClicked();
	void diffBetweenClicked();
	void deleteBranch();
	void checkoutBranch();
	void createBranch();
	void getBranchesList();
	void mergeBranch();

	bool isCompactMode();

private slots:
	void onInitComplete(bool success);
	void onCloneComplete(bool success);
	void onCommitComplete(bool success);
	void onRemoteComplete(bool success);
	void onPushComplete(bool success);
	void onPullComplete(bool success);
	void onResetComplete(bool success);
	void onCheckoutBranchComplete(bool success);
	void onCreateBranchComplete(bool success);
	void onDeleteBranchComplete(bool success);
	void onMergeBranchesComplete(bool success);
	void onStatusComplete(const QString &answer, bool success);
	void onLogComplete(const QString &answer, bool success);
	void onRemoteListComplete(const QString &answer, bool success);
	void modeChanged(bool compactMode);
	void removeClosedTab(QWidget *widget);
	void activeTabChanged(const TabInfo &info);

private:
	void initActions();
	void showMessage(const QString &message);

	bool mIsFullScreen;
	bool mIsCompactMode;
	QList<qReal::ActionInfo> mMenu;
	GitPlugin *mPlugin;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowIface;
	qReal::ProjectManagementInterface *mProjectManager;
	qrRepo::RepoControlInterface *mRepoApi;
	ui::PreferencesVersioningPage *mPreferencesPage;
	QList<QWidget *> mDiffWidgets;
	TransparentMode *mCompactMode;
	qReal::SystemEvents *mSystemEvents;
};

}
}
