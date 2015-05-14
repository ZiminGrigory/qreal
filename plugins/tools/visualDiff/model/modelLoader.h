#pragma once

#include <QtCore/QObject>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrutils/versioningUtils/briefVersioningInterface.h>

#include "diffModel.h"


namespace versioning
{
namespace details
{

class ModelLoader : public QObject
{
	Q_OBJECT

public:
	ModelLoader(
		qReal::BriefVersioningInterface *vcs
		, qReal::ErrorReporterInterface *errorReporter
		, qReal::EditorManagerInterface *editorManager
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
	);

	// for diff
	void startModelLoading(const QString &targetProject);
	void startModelLoading(const QString &repoRevision, const QString &targetProject);
	void startModelLoading(
		const QString &oldRepoRevision
		, const QString &newRepoRevision
		, const QString &targetProject
	);
	//for condlicts
	void startModelLoading2(const QString &repoUrl, const QString &branch, const QString &targetProject);
	void startModelLoading2(const QString &branch, const QString &targetProject);

signals:
	void modelLoaded(DiffModel *model);
	void internalModelLoaded(qReal::models::Models *model);

private slots:
	void onDownloadingComplete(bool success, const QString &targetProject);
	void onOldModelLoaded(qReal::models::Models *model);
	void onNewModelLoaded(qReal::models::Models *model);

private:
	QString tempProject() const;

	qReal::models::Models *loadFromDisk(const QString &targetProject);

	void finishModelLoading();

	void reportError(const QString &message);
	void reportWarning(const QString &message);

	qReal::BriefVersioningInterface *mVcs;
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::EditorManagerInterface *mEditorManager;
	qrRepo::WorkingCopyManagementInterface *mWorkingCopyManager;
	QString mTempProject;

	qReal::models::Models *mOldModel;
	qReal::models::Models *mOldActiveModel;
	qReal::models::Models *mNewModel;
	QString mRepoUrl;
	QString mNewRevision;
	QString mBranch;
	QString mTargetProject;
	bool mIsForConflicts;
};

}
}
