#pragma once

#include <qrutils/versioningUtils/externalClientPluginBase.h>
#include <qrgui/plugins/toolPluginInterface/customizer.h>

#include "viewInteraction.h"

namespace git
{

class GitPlugin : public qReal::versioning::ExternalClientPluginBase
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.versioning.git")
public:
	GitPlugin();

	virtual ~GitPlugin();

	QString pathToGit() const;

	// QReal plugin interface
	virtual qReal::Customizer* customizationInterface();
	virtual void updateSettings();
	virtual QList<qReal::ActionInfo> actions();
	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QPair<QString, qReal::gui::PreferencesPage *> preferencesPage();

	// Working copy inspection
	virtual bool onFileAdded(const QList<QString> &list, const QString &workingDir);
	virtual bool onFileRemoved(const QList<QString> &list, const QString &workingDir);
	virtual bool onFileChanged(const QList<QString> &list, const QString &workingDir);

	// Brief VCS interface
	virtual void beginWorkingCopyDownloading(
		const QString &repoAddress
		, const QString &targetProject
		, const QString &branch = QString()
		, const QString &commitId = "-1"
		, bool quiet = false
	);
	virtual void beginWorkingCopyUpdating(const QString &targetProject = QString());
	virtual void beginChangesSubmitting(
		const QString &description
		, const QString &targetProject = QString()
		, bool quiet = false
	);
	virtual bool reinitWorkingCopy(const QString &targetProject = QString());
	virtual QString information(const QString &targetProject = QString());
	virtual QString commitId(const QString &targetProject = QString());
	virtual QString remoteRepositoryUrl(const QString &targetProject = QString());
	virtual bool isMyWorkingCopy(
		const QString &directory = QString()
		, bool quiet = false
		, bool prepareAndProcess = false
	);
	virtual QString friendlyName();
	bool clientExist();

	virtual QString getLog(const QString &format = QString(), bool quiet = false);
	virtual void setVersion(QString hash, bool quiet = false);
	void setDiffViewerInterface(qReal::DiffPluginInterface *diffInterface);

public slots:
	void doInit(const QString &targetFolder = QString()
		, bool prepareAndProcess = true
		, bool quiet = false
	);
	void startClone(const QString &from = QString(), const QString &targetProject = QString());
	void startCommit(
		const QString &message
		, const QString &from  = QString()
		, const QString &sourceProject = QString()
		, bool quiet = false
	);
	void doRemote(const QString &remote, const QString &adress, const QString &targerFolder = QString());
	void startPush(
		const QString &remote
		, const QString &sourceProject = QString()
		, const QString &targetFolder = QString()
	);
	void startPull(const QString &remote, const QString &branch = QString(), const QString &targetFolder = QString());
	void startReset(const QString &hash = QString(), const QString &targetFolder = QString(), bool quiet = false);
	bool doAdd(const QString &what, const QString &targetFolder, bool force = true);
	bool doRemove(const QString &what, bool prepare = true, bool process = true, bool force = true);
	bool doClean(const QString &targetProject = QString());
	QString doStatus(const QString &targetProject = QString());
	QString doLog(const QString &format = QString(), bool quiet = false, bool showDialog = false);
	QString doRemoteList();
	void doAfterOperationIsFinished(QVariant const &tag, bool result);
	void checkClientInstalling();

	//For branches
	void deleteBranch(const QString &branchName);
	void startCheckoutBranch(const QString &branchName, const QString &targetFolder = QString());
	void createBranch(const QString &branchName);
	QString getBranchesList();
	void startMergeBranch(const QString &targetBranchName);

	//Difference between commits
	void showDiff(const QString &targetProject, QWidget *widget, bool compactMode = true);
	void showDiff(const QString &oldHash, const QString &newHash
				  , const QString &targetProject, QWidget *widget, bool compactMode = true);
	void showDiff(const QString &oldhash, const QString &targetProject, QWidget *widget, bool compactMode = true);

signals:
	void workingCopyDownloaded(bool success, const QString &targetProject);
	void workingCopyUpdated(bool success);
	void changesSubmitted(bool success);
	void clientInstalled(QString client, bool exist);

	void initComplete(bool success);
	void cloneComplete(bool success);
	void commitComplete(bool success);
	void remoteComplete(bool success);
	void pushComplete(bool success);
	void pullComplete(bool success);
	void resetComplete(bool success);
	void cleanComplete(bool success);
	void addComplete(bool success);
	void checkoutComplete(bool success);
	void deleteBranchComplete(bool success);
	void createBranchComplete(bool success);
	void mergeBranchesComplete(bool success);
	void removeComplete(bool success);
	void statusComplete(const QString &answer, bool success);
	void logComplete(const QString &answer, bool success);
	void remoteListComplete(const QString &answer, bool success);
	void operationComplete(const QString &name, bool success);

protected:
	// External client overloads
	friend class TransparentMode;
	virtual int timeout() const;
	virtual QString tempFolder() const;

private:
	void onCloneComplete(bool result, const QString &targetProject, bool quiet);
	void onRemoteComplete(bool result);
	void onPushComplete(bool result);
	void onPullComplete(bool result, const QString &remote, const QString &branch);
	void onResetComplete(bool result, bool quiet);
	void onCheckoutComplete(bool result);
	void onMergeComplete(bool result, const QString &branch);

	QString &getFilePath(QString &adress);
	QString getUsername();
	QString getPassword();
	void doUserNameConfig();
	void doUserEmailConfig();

	details::ViewInteraction *mViewInteraction;
	QString mTempDir;
	qReal::DiffPluginInterface *mDiffInterface;
};
}
