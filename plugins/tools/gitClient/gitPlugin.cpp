#include "gitPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

#include <qrkernel/settingsManager.h>
#include <qrutils/fileSystemUtils.h>

#include "tags.h"

using namespace git;
using namespace qReal::versioning;

QString const tempFolderName = "tempGit";
int const defaultTimeout = 30000;

// some defaults for function invokeOperation are empty, alias for convenience
bool const needPreparation = true;
bool const checkWorkingDir = true;
bool const needProcessing = true;
QString const dummyWorkingDir = "";
QString const dummySourceProject = "";
QString const dummyTargetProject = "";
QString const dummyBranch = "";

GitPlugin::GitPlugin()
	: mViewInteraction(new details::ViewInteraction(this))
	, mTempDir(qApp->applicationDirPath() + "/" + tempFolderName)
{
	qReal::SettingsManager::instance()->setValue("gitTempDir", mTempDir);
	setPathToClient(pathToGit());

	connect(this, SIGNAL(operationIsFinished(QVariant, bool)), SLOT(doAfterOperationIsFinished(QVariant, bool)));
}

GitPlugin::~GitPlugin()
{
}

// QReal plugin interface
QString GitPlugin::pathToGit() const
{
	return qReal::SettingsManager::value("pathToGitClient", "").toString();
}

qReal::Customizer *GitPlugin::customizationInterface()
{
	return nullptr;
}

void GitPlugin::updateSettings()
{
	setPathToClient(pathToGit());
}

QList<qReal::ActionInfo> GitPlugin::actions()
{
	return mViewInteraction->actions();
}

void GitPlugin::init(qReal::PluginConfigurator const &configurator)
{
	ExternalClientPluginBase::init(configurator);
	mViewInteraction->init(configurator);
}

QPair<QString, qReal::gui::PreferencesPage *> GitPlugin::preferencesPage()
{
	return mViewInteraction->preferencesPage();
}

bool GitPlugin::onFileAdded(const QList<QString> &list, QString const &workingDir)
{
//	return doAdd(filePath, workingDir);
	Q_UNUSED(list)
	Q_UNUSED(workingDir)
	return true;
}

bool GitPlugin::onFileRemoved(const QList<QString> &list, QString const &workingDir)
{
	Q_UNUSED(workingDir)
	Q_UNUSED(list)
	return true;
}

bool GitPlugin::onFileChanged(const QList<QString> &list, QString const &workingDir)
{
//	return doAdd(filePath, workingDir);
	Q_UNUSED(list)
	Q_UNUSED(workingDir)
	return true;
}

void GitPlugin::beginWorkingCopyDownloading(
	const QString &repoAddress
	, const QString &targetProject
	, const QString &branch
	, const QString &commitId
	, bool quiet
)
{
	///@to do: make it more readable and think about mass code refactoring
	bool success = true;
	bool remoteUrl = repoAddress.startsWith("https");
	if (!(branch != QString() && remoteUrl)) {
		if (commitId.isEmpty()) {
			success = invokeOperation(
				QStringList() << "init"
				, needPreparation
				, dummyWorkingDir
				, !checkWorkingDir
				, needProcessing
				, targetProject
				, dummySourceProject
				, quiet
			);
		} else {
			QString hash = commitId;
			if(commitId == "-1") {
				hash = "HEAD";
			}

			success = invokeOperation(
				QStringList() << "reset" << hash << "--hard"
				, needPreparation
				, dummyWorkingDir
				, checkWorkingDir
				, needProcessing
				, targetProject
				, repoAddress
				, quiet
			);
		}
	} else {
		if (!remoteUrl) {
			success = invokeOperation(
				QStringList() << "checkout" << branch
				, needPreparation
				, dummyWorkingDir
				, !checkWorkingDir
				, needProcessing
				, targetProject
				, dummySourceProject
				, quiet
			);
		} else {
			// problem with QProcessPrivate.
			// If dir doesn't exist, and git clone make it , the QProcessPrivate ends with error
			QDir dir;
			dir.mkdir(mTempDir);
			success = invokeOperation(
				QStringList() << "clone" << "-b" << branch << "--single-branch" << repoAddress << mTempDir << "-q"
				, !needPreparation
				, dummyWorkingDir
				, !checkWorkingDir
				, needProcessing
				, targetProject
				, dummySourceProject
				, quiet
			);
		}
	}

	emit workingCopyDownloaded(success, targetProject);
}

void GitPlugin::beginWorkingCopyUpdating(QString const &targetProject)
{
	startPull(tempFolder(), dummyBranch, targetProject);
}

void GitPlugin::beginChangesSubmitting(QString const &description, QString const &targetProject, bool quiet)
{
	startCommit(description, tempFolder(), targetProject, quiet);
}

bool GitPlugin::reinitWorkingCopy(QString const &targetProject)
{
	startReset();
	doInit();
	return doClean(targetProject);
}

QString GitPlugin::information(QString const &targetProject)
{
	return doStatus(targetProject);
}

QString GitPlugin::commitId(QString const &targetProject)
{
	QStringList args {"log", "--pretty=%H", "-1"};
	invokeOperation(args, needPreparation, dummyWorkingDir, checkWorkingDir, needProcessing, targetProject);
	return standartOutput().remove("\"");
}

QString GitPlugin::remoteRepositoryUrl(QString const &targetProject)
{
	return targetProject;
}

bool GitPlugin::isMyWorkingCopy(QString const &directory, bool quiet, bool prepareAndProcess)
{
	QStringList infoArgs{"remote", "show"};
	return invokeOperation(
		infoArgs
		, prepareAndProcess
		, directory
		, !checkWorkingDir
		, prepareAndProcess
		, dummyTargetProject
		, dummySourceProject
		, !quiet
	);
}

int GitPlugin::timeout() const
{
	return qReal::SettingsManager::value("gitClientTimeout", defaultTimeout).toInt();
}

QString GitPlugin::tempFolder() const
{
	return qReal::SettingsManager::value("gitTempDir", mTempDir).toString();
}

void GitPlugin::checkClientInstalling()
{
	emit clientInstalled(friendlyName(), clientExist());
}

void GitPlugin::deleteBranch(const QString &branchName)
{
	emit deleteBranchComplete(invokeOperation(QStringList() << "branch" << "-D" << branchName));
}

void GitPlugin::startCheckoutBranch(const QString &branchName, const QString &targetFolder)
{
	const Tag tagStruct("checkoutBranch", branchName);
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(QStringList() << "checkout" << branchName, tagVariant, needPreparation, targetFolder);
}

void GitPlugin::createBranch(const QString &branchName)
{
	emit createBranchComplete(invokeOperation(QStringList() << "branch" << branchName));
}

QString GitPlugin::getBranchesList()
{
	invokeOperation(QStringList() << "branch" << "-l");
	return standartOutput();
}


void GitPlugin::startMergeBranch(const QString &targetBranchName)
{
	const Tag tagStruct("merge", targetBranchName);
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(QStringList() << "merge" << targetBranchName, tagVariant, needPreparation);
}

QString GitPlugin::friendlyName()
{
	return "Git Plugin";
}

//non secure, to do: must be as transaction - if break - need reset
void GitPlugin::setVersion(QString hash, bool quiet)
{
	bool nPrp = needPreparation;
	bool nPrc = needProcessing;
	bool cWD = checkWorkingDir;

	//because if we have unsaved changes, it would be a crash
	if (!this->isMyWorkingCopy(QString(), quiet, nPrp)){
		this->doInit(QString(), true);
	}
	this->beginChangesSubmitting("version was saved in a transparent mode", dummyTargetProject, quiet);

	//some magic with git branches
	QStringList args;
	args << "branch" << "tmp";
	invokeOperation(args, nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "checkout" << "tmp";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "reset" << "--hard" << hash;
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "checkout" << "master";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "cherry-pick" << "tmp" << "--allow-empty" << "--keep-redundant-commits" << "--strategy=ours";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "branch" << "branchTEMP";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "reset" << "--hard" << "tmp";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "reset" << "--soft" << "branchTEMP";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "commit" << "--amend" << "-m" << "version was saved in a transparent mode";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "branch" << "-D" << "branchTEMP";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, !nPrc, dummyTargetProject, dummySourceProject, !quiet);

	args.clear();
	args << "branch" << "-D" << "tmp";
	invokeOperation(args, !nPrp, dummyWorkingDir, !cWD, nPrc, dummyTargetProject, dummySourceProject, !quiet);
}

void GitPlugin::setDiffViewerInterface(qReal::DiffPluginInterface *diffInterface)
{
	mDiffInterface = diffInterface;
}

bool GitPlugin::clientExist()
{
	QProcess *process = new QProcess;
	process->start(pathToGit(), QStringList() << "--version");
	process->waitForFinished();
	QByteArray answer = process->readAllStandardOutput();
	bool res = answer.startsWith(QString("git").toLocal8Bit());
	qReal::SettingsManager::setValue("gitClientExist", res);
	if (res) {
		setPathToClient(pathToGit());
	}
	delete process;
	return res;
}

QString GitPlugin::getLog(const QString &format, bool quiet)
{
	return doLog(format, quiet);
}

void GitPlugin::doInit(const QString &targetFolder, bool prepareAndProcess, bool quiet)
{
	bool isInit = isMyWorkingCopy(targetFolder, true, prepareAndProcess);
	if (!isInit){
		QStringList arguments{"init"};
		bool result = invokeOperation(
			arguments
			, needPreparation
			, targetFolder
			, !checkWorkingDir
			, needProcessing
			, dummyTargetProject
			, dummySourceProject
			, !quiet
		);

		if (result) {
			arguments.clear();
			arguments << "add" << "-A";
			invokeOperation(
				arguments
				, needPreparation
				, targetFolder
				, !checkWorkingDir
				, needProcessing
				, dummyTargetProject
				, dummySourceProject
				, !quiet
			);

			doUserEmailConfig();
			doUserNameConfig();
		}

		if (!quiet){
			emit initComplete(result);
		}
	} else {
		emit initComplete(isInit);
	}
}

void GitPlugin::startClone(const QString &from, const QString &targetProject)
{
	QStringList arguments{"clone", from, mTempDir, "-q"};

	const Tag tagStruct("clone", targetProject);
	// problem with QProcessPrivate.
	// If dir doesn't exist, and git clone make it , the QProcessPrivate ends with error
	QDir dir;
	dir.mkdir(mTempDir);
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	QString workingDir = qApp->applicationDirPath();
	invokeOperationAsync(arguments, tagVariant, !needPreparation, workingDir, targetProject, !checkWorkingDir);
}

void GitPlugin::startCommit(const QString &message
		, const QString &from
		, const QString &sourceProject
		, bool quiet
)
{
	Q_UNUSED(from)
	QStringList arguments;
	arguments << "add" << "-f" << "-A";
	bool result = invokeOperation(
		arguments
		, needPreparation
		, dummyWorkingDir
		, checkWorkingDir
		, needProcessing
		, dummyWorkingDir
		, sourceProject
		, !quiet
	);

	if (result){
		arguments.clear();
		arguments << "commit" << "-m" << message;
		result = invokeOperation(
			arguments
			, needPreparation
			, dummyWorkingDir
			, !checkWorkingDir
			, needProcessing
			, dummyTargetProject
			, sourceProject
			, !quiet
		);
	}

	if (!quiet){
		emit commitComplete(result);
		emit operationComplete("commit", result);
	}
}

void GitPlugin::doRemote(const QString &remote, const QString &adress, const QString &targetFolder)
{
	QStringList arguments{"remote", "add", remote, adress};

	bool const result = invokeOperation(
		arguments
		, needPreparation
		, dummyWorkingDir
		, !checkWorkingDir
		, needProcessing
		, targetFolder
		, dummySourceProject
	);

	emit addComplete(result);
	emit operationComplete("add", result);
	emit operationComplete("remote", result);
}

void GitPlugin::startPush(
	const QString &remote
	, const QString &sourceProject
	, const QString &targetFolder
)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;

	invokeOperation(QStringList() << "remote" << "-v");
	QStringList tmp = standartOutput().split(QChar('\n'));

	while (!tmp.first().startsWith(remote) && !tmp.isEmpty()){
		tmp.removeFirst();
	}

	QString repo = tmp.first();
	repo.remove(remote, Qt::CaseSensitive);
	repo.remove(" ", Qt::CaseSensitive);
	repo.remove("\t", Qt::CaseSensitive);
	repo.remove("(fetch)", Qt::CaseSensitive);
	repo.remove("(push)", Qt::CaseSensitive);
	repo.remove("https://", Qt::CaseSensitive);

	QStringList arguments{"push" ,"https://" + getUsername() + ":" + getPassword() + "@" + repo, "master", "-q"};

	const Tag tagStruct("push");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, needPreparation, targetDir, sourceProject);
}

void GitPlugin::startPull(const QString &remote, const QString &branch, const QString &targetFolder)
{
	QStringList arguments{"pull", remote, branch};

	const Tag tagStruct("pull", remote, branch);
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, needPreparation, targetFolder, dummySourceProject, !checkWorkingDir);
}

void GitPlugin::startReset(const QString &hash, const QString &targetFolder, bool quiet)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments{"reset", hash, "--hard"};

	const Tag tagStruct("reset", QString(), QString(), quiet);
	QVariant tagVar;
	tagVar.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVar, needPreparation, targetDir, dummySourceProject, checkWorkingDir, !quiet);
}

void GitPlugin::doUserNameConfig()
{
	QString const enabledKey = qReal::versioning::ui::AuthenticationSettingsWidget::enabledSettingsName("git");
	QString const usernameKey = qReal::versioning::ui::AuthenticationSettingsWidget::usernameSettingsName("git");
	QString username = "qReal";

	bool const authenticationEnabled = qReal::SettingsManager::value(enabledKey, false).toBool();
	if (authenticationEnabled && !qReal::SettingsManager::value(usernameKey, false).toString().isEmpty()) {
			username = qReal::SettingsManager::value(usernameKey, false).toString();
	}

	QStringList arguments{"config", "user.name", "\"" + username + "\""};
	bool result = invokeOperation(arguments);
	emit operationComplete("userName", result);
}

void GitPlugin::doUserEmailConfig()
{
	QString const enabledKey = qReal::versioning::ui::AuthenticationSettingsWidget::enabledSettingsName("git");
	QString const emailKey = qReal::versioning::ui::AuthenticationSettingsWidget::emailSettingsName("git");
	QString email = "qReal@qrealmail.com";

	bool const authenticationEnabled = qReal::SettingsManager::value(enabledKey, false).toBool();
	if (authenticationEnabled
		&& !qReal::SettingsManager::value(emailKey, false).toString().isEmpty())
	{
		email = qReal::SettingsManager::value(emailKey, false).toString();
	}

	QStringList arguments{"config", "user.email", email};
	bool result = invokeOperation(arguments);
	emit operationComplete("userEmail", result);
}

bool GitPlugin::doAdd(const QString &what, const QString &targetFolder, bool force)
{
	QStringList arguments{"add", what};
	if (force){
		arguments.append("-f");
	}
	QString path = what;
	path = getFilePath(path);

	bool result = invokeOperation(arguments, needPreparation, path, !checkWorkingDir, needProcessing, targetFolder);
	emit addComplete(result);
	emit operationComplete("add", result);
	return result;
}

QString &GitPlugin::getFilePath(QString &adress)
{
	int pos = adress.lastIndexOf("/");
	int len = adress.length();
	return adress.remove(pos, len - pos);
}

bool GitPlugin::doRemove(const QString &what, bool prepare, bool process, bool force)
{
	QStringList arguments{"rm", what, "-r"};

	if (force) {
		arguments.append("-f");
	}

	QString path = what;
	path = getFilePath(path);

	bool result = invokeOperation(arguments, prepare, path, !checkWorkingDir, process);
	emit removeComplete(result);
	emit operationComplete("rm", result);
	return result;
}

bool GitPlugin::doClean(const QString &targetProject)
{
	QStringList arguments{"clean", "-xdf"};
	bool result = invokeOperation(
		arguments
		, needPreparation
		, dummyWorkingDir
		, checkWorkingDir
		, needProcessing
		, targetProject
	);

	emit cleanComplete(result);
	emit operationComplete("clean", result);
	return result;
}

QString GitPlugin::doStatus(const QString &targetProject)
{
	bool result = invokeOperation(
		QStringList() << "status"
		, needPreparation
		, dummyWorkingDir
		, checkWorkingDir
		, needProcessing
		, targetProject
	);
	QString answer = standartOutput();
	emit statusComplete(answer, result);
	return answer;
}

QString GitPlugin::doLog(const QString &format, bool quiet, bool showDialog)
{
	QStringList arguments{"log"};
	if (format.size() != 0) {
		arguments << format;
	}

	bool result = invokeOperation(
		arguments
		, needPreparation
		, dummyWorkingDir
		, checkWorkingDir
		, needProcessing
		, dummyTargetProject
		, dummySourceProject
		, !quiet
	);

	QString answer = standartOutput();

	if (showDialog){
		emit logComplete(answer, result);
	}

	return answer;
}

QString GitPlugin::doRemoteList()
{
	bool result = invokeOperation(QStringList() << "remote" << "-v");
	QString answer = standartOutput();
	emit remoteListComplete(answer, result);
	return answer;
}

void GitPlugin::doAfterOperationIsFinished(QVariant const &tag, bool result)
{
	Tag tagStruct = tag.value<Tag>();
	if (tagStruct.operation == "clone"){
		onCloneComplete(result, tagStruct.stringTagOption1, tagStruct.boolTag);
	} else if (tagStruct.operation == "push"){
		onPushComplete(result);
	} else if (tagStruct.operation == "pull"){
		onPullComplete(result, tagStruct.stringTagOption1, tagStruct.stringTagOption2);
	} else if (tagStruct.operation == "reset"){
		onResetComplete(result, tagStruct.boolTag);
	} else if (tagStruct.operation == "checkoutBranch"){
		onCheckoutComplete(result);
	} else if (tagStruct.operation == "merge"){
		onMergeComplete(result, tagStruct.stringTagOption1);
	}
}

QString GitPlugin::getUsername()
{
	QString const usernameKey = qReal::versioning::ui::AuthenticationSettingsWidget::usernameSettingsName("git");
	QString const username = qReal::SettingsManager::value(usernameKey, false).toString();
	return username;
}

QString GitPlugin::getPassword()
{
	QString const passwordKey = qReal::versioning::ui::AuthenticationSettingsWidget::passwordSettingsName("git");
	QString const password = qReal::SettingsManager::value(passwordKey, false).toString();
	return password;
}

void GitPlugin::onCloneComplete(bool result, const QString& targetProject , bool quiet)
{
	processWorkingCopy(targetProject);
	doUserNameConfig();
	doUserEmailConfig();
	if (!quiet) {
		emit cloneComplete(result);
		emit operationComplete("clone", result);
	}
}

void GitPlugin::onPushComplete(bool result)
{
	processWorkingCopy();
	emit pushComplete(result);
	emit operationComplete("push", result);
}

void GitPlugin::onPullComplete(bool result, const QString &remote, const QString &branch)
{
	if (result) {
		processWorkingCopy();
		emit pullComplete(result);
		emit operationComplete("pull", result);
	} else {
		QWidget *widget = mViewInteraction->makeDiffTab(tr("pullConflicts"));
		mDiffInterface->solvePull(remote, branch, mViewInteraction->currentProject(), widget);
	}
}

void GitPlugin::onMergeComplete(bool result, const QString &branch)
{
	if (result) {
		processWorkingCopy();
		emit pullComplete(result);
		emit operationComplete("pull", result);
	} else {
		QWidget *widget = mViewInteraction->makeDiffTab(tr("mergeConflicts"));
		mDiffInterface->solveMerge(branch, mViewInteraction->currentProject(), widget);
	}
}

void GitPlugin::onResetComplete(bool result, bool quiet)
{
	processWorkingCopy();
	if (!quiet) {
		emit resetComplete(result);
		emit operationComplete("reset", result);
	}
	emit workingCopyUpdated(result);
}

void GitPlugin::onCheckoutComplete(bool result)
{
	processWorkingCopy();
	emit operationComplete("checkout", result);
	emit checkoutComplete(result);
}

void GitPlugin::showDiff(const QString &oldHash, const QString &newHash, const QString &targetProject, QWidget *widget
, bool compactMode)
{
	mDiffInterface->showDiff(oldHash, newHash, targetProject, widget, compactMode);
}

void GitPlugin::showDiff(const QString &oldhash, const QString &targetProject, QWidget *widget, bool compactMode)
{
	mDiffInterface->showDiff(oldhash, targetProject, widget, compactMode);
}

void GitPlugin::showDiff(const QString &targetProject, QWidget *widget, bool compactMode)
{
	mDiffInterface->showDiff(targetProject, widget, compactMode);
}
