#include "modelLoader.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QString>

#include <qrutils/fileSystemUtils.h>

using namespace versioning;
using namespace versioning::details;

QString const tempProjectName = "tempDiff.qrs";

ModelLoader::ModelLoader(
		qReal::BriefVersioningInterface *vcs
		, qReal::ErrorReporterInterface *errorReporter
		, qReal::EditorManagerInterface *editorManager
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
		)
	: mVcs(vcs)
	, mErrorReporter(errorReporter)
	, mEditorManager(editorManager)
	, mWorkingCopyManager(workingCopyManager)
	, mTempProject(qApp->applicationDirPath() + "/" + tempProjectName)
{
	qReal::SettingsManager::setValue("diffTempProject", mTempProject);

	connect(mVcs, SIGNAL(workingCopyDownloaded(bool, QString)), this, SLOT(onDownloadingComplete(bool, QString)));
}

QString ModelLoader::tempProject() const
{
	return qReal::SettingsManager::value("diffTempProject", mTempProject).toString();
}

void ModelLoader::startModelLoading(const QString &targetProject)
{
	mBranch = QString();
	mIsForConflicts = false;
	mTargetProject = targetProject;
	mOldModel = loadFromDisk(targetProject);
	if (!mOldModel) {
		emit modelLoaded(nullptr);
		return;
	}

	QString const repoUrl = mVcs->remoteRepositoryUrl(targetProject);
	if (repoUrl.isEmpty()) {
		emit modelLoaded(nullptr);
		return;
	}

	connect(
		this
		, SIGNAL(internalModelLoaded(qReal::models::Models*))
		, this
		, SLOT(onNewModelLoaded(qReal::models::Models*))
	);

	mVcs->beginWorkingCopyDownloading(repoUrl, tempProject(), "", "-1");
}

void ModelLoader::startModelLoading(const QString &repoRevision, const QString &targetProject)
{
	mBranch = QString();
	mIsForConflicts = false;
	mTargetProject = targetProject;
	mOldModel = loadFromDisk(targetProject);
	if (!mOldModel) {
		emit modelLoaded(nullptr);
		return;
	}

	QString const &repoUrl = mVcs->remoteRepositoryUrl(targetProject);
	if (repoUrl.isEmpty()) {
		emit modelLoaded(nullptr);
		return;
	}

	connect(
		this
		, SIGNAL(internalModelLoaded(qReal::models::Models*))
		, this
		, SLOT(onNewModelLoaded(qReal::models::Models*))
	);

	mVcs->beginWorkingCopyDownloading(repoUrl, tempProject(), QString(), repoRevision);
}

void ModelLoader::startModelLoading(
	const QString &oldRepoRevision
	, const QString &newRepoRevision
	, const QString &targetProject
)
{
	mRepoUrl = mVcs->remoteRepositoryUrl(targetProject);
	mNewRevision = newRepoRevision;
	mBranch = QString();
	mIsForConflicts = false;
	mTargetProject = targetProject;
	if (mRepoUrl.isEmpty()) {
		emit modelLoaded(nullptr);
		return;
	}

	connect(
		this
		, SIGNAL(internalModelLoaded(qReal::models::Models*))
		, this
		, SLOT(onOldModelLoaded(qReal::models::Models*))
	);

	mVcs->beginWorkingCopyDownloading(mRepoUrl, tempProject(), QString(), oldRepoRevision);
}

void ModelLoader::startModelLoading2(const QString &repoUrl, const QString &branch, const QString &targetProject)
{
	mRepoUrl = repoUrl;
	mBranch = branch;
	mIsForConflicts = true;
	mTargetProject = targetProject;
	mNewRevision = "-1";
	connect(
		this
		, SIGNAL(internalModelLoaded(qReal::models::Models*))
		, this
		, SLOT(onOldModelLoaded(qReal::models::Models*))
	);

	mVcs->beginWorkingCopyDownloading(mVcs->remoteRepositoryUrl(targetProject), tempProject(), QString(), "-1");
}

void ModelLoader::startModelLoading2(const QString &branch, const QString &targetProject)
{
	mRepoUrl = mVcs->remoteRepositoryUrl(targetProject);;
	mBranch = branch;
	mIsForConflicts = true;
	mTargetProject = targetProject;
	mNewRevision = "-1";
	connect(
		this
		, SIGNAL(internalModelLoaded(qReal::models::Models*))
		, this
		, SLOT(onOldModelLoaded(qReal::models::Models*))
	);

	mVcs->beginWorkingCopyDownloading(mRepoUrl, tempProject(), QString(), "-1");
}

qReal::models::Models *ModelLoader::loadFromDisk(const QString &targetProject)
{
	qReal::models::Models *result = new qReal::models::Models(targetProject, *mEditorManager);
	result->repoControlApi().open(targetProject);
	result->reinit();
	return result;
}

void ModelLoader::onDownloadingComplete(bool success, const QString &targetProject)
{
	if (!success) {
		emit modelLoaded(nullptr);
	}
	emit internalModelLoaded(loadFromDisk(targetProject));
}

void ModelLoader::onOldModelLoaded(qReal::models::Models *model)
{
	mOldModel = model;
	disconnect(this, SLOT(onOldModelLoaded(qReal::models::Models*)));
	if (!mOldModel) {
		emit modelLoaded(nullptr);
		mOldActiveModel = nullptr;
	} else if (mIsForConflicts) {
		mOldActiveModel = loadFromDisk(mTargetProject);
	}

	qReal::FileSystemUtils::removeFile(tempProject());
	connect(
		this
		, SIGNAL(internalModelLoaded(qReal::models::Models*))
		, this
		, SLOT(onNewModelLoaded(qReal::models::Models*))
	);

	mVcs->beginWorkingCopyDownloading(mRepoUrl, tempProject(), mBranch, mNewRevision);
}

void ModelLoader::onNewModelLoaded(qReal::models::Models *model)
{
	mNewModel = model;
	disconnect(this, SLOT(onNewModelLoaded(qReal::models::Models*)));
	finishModelLoading();
}

void ModelLoader::finishModelLoading()
{
	DiffModel *result = nullptr;
	if (mOldModel && mNewModel) {
		result = new DiffModel(mOldModel, mNewModel, mOldActiveModel);
	}

	qReal::FileSystemUtils::removeFile(tempProject());
	emit modelLoaded(result);
}

void ModelLoader::reportError(const QString &message)
{
	mErrorReporter->addError(message);
}

void ModelLoader::reportWarning(const QString &message)
{
	mErrorReporter->addWarning(message);
}
