#include "threadsValidator.h"

using namespace generatorBase;

ThreadsValidator::ThreadsValidator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, qReal::ErrorReporterInterface &errorReporter)
	: RobotsDiagramVisitor(repo, customizer)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
{
}

bool ThreadsValidator::validate(const qReal::Id &startNode, const QString &threadId)
{
	mBlockThreads.clear();
	mNoErrors = true;

	mStartNode = startNode;
	mThreadId = threadId;
	mBlockThreads[mStartNode] = mThreadId;
	mThreadCreatingBlocks[mThreadId] = mStartNode;

	mSecondStage = false;
	do {
		mSomethingChanged = false;
		startSearch(mStartNode);
	} while (mSomethingChanged && mNoErrors);

	if (mNoErrors) {
		mSecondStage = true;
		startSearch(mStartNode);
	}

	return mNoErrors;
}

void ThreadsValidator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void ThreadsValidator::visitFinal(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void ThreadsValidator::visitConditional(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void ThreadsValidator::visitLoop(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);

	if (mSecondStage) {
		for (const LinkInfo &link : links) {
			if (guardOf(link.linkId) != iterationGuard || !link.connected) {
				continue;
			}

			mVisitedBlocks.clear();
			if (foundForks(link.target)) {
				error(QObject::tr("Creation and joining of threads in a loop is forbidden"), id);
			}
		}
	}
}

void ThreadsValidator::visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void ThreadsValidator::visitUnknown(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void ThreadsValidator::visitGeneral(const qReal::Id &id, const QList<LinkInfo> &links)
{
	if (mSecondStage) {
		return;
	}

	if (mBlockThreads[id] == "@@unknown@@") {
		unknownThread(links);
		return;
	}

	QSet<QString> incomingThreads;
	for (const qReal::Id &link : mRepo.incomingLinks(id)) {
		const qReal::Id previousBlock = mRepo.otherEntityFromLink(link, id);
		if (mBlockThreads.contains(previousBlock)) {
			if (previousBlock.element() == "Fork") {
				incomingThreads.insert(mRepo.stringProperty(link, "Guard"));
			} else {
				incomingThreads.insert(mBlockThreads[previousBlock]);
			}
		}
	}

	if ((incomingThreads.size() != 1 && mStartNode != id) || incomingThreads.contains("@@unknown@@")) {
		unknownThread(links);
		return;
	}

	for (const LinkInfo &link : links) {
		checkForConnected(link);

		const qReal::Id nextBlock = link.target;
		if (!mBlockThreads.contains(nextBlock)) {
			mSomethingChanged = true;
			mBlockThreads[nextBlock] = mBlockThreads[id];
		} else if (mBlockThreads[nextBlock] != mBlockThreads[id] && mBlockThreads[nextBlock] != "@@unknown@@"
				&& nextBlock.element() != "Join") {
			mSomethingChanged = true;
			mBlockThreads[nextBlock] = "@@unknown@@";
		}
	}
}

void ThreadsValidator::visitJoin(const qReal::Id &id, QList<LinkInfo> &links)
{
	for (const qReal::Id &link : mRepo.incomingLinks(id)) {
		if (mBlockThreads[mRepo.otherEntityFromLink(link, id)] == "@@unknown@@") {
			error(QObject::tr("Trying to join a thread with an unknown id. Possible causes: "
					"calling fork from a subprogram or trying to merge two threads without a join"), id);
			return;
		}
	}

	if (links.size() != 1) {
		error(QObject::tr("Join block must have exactly one outgoing link"), id);
		return;
	}

	mBlockThreads[id] = mRepo.stringProperty(links[0].linkId, "Guard");
	if (mBlockThreads[id].isEmpty()) {
		error(QObject::tr("Guard property of a link outgoing from a join must contain an id "
						  "of one of joined threads"), links[0].linkId);
		return;
	}

	mVisitedBlocks.clear();
	if (achiavable(id, id)) {
		error(QObject::tr("Joining threads in a loop is forbidden"), id);
		return;
	}

	checkForConnected(links[0]);
	const qReal::Id nextBlock = links[0].target;

	if (mBlockThreads.contains(nextBlock) && mBlockThreads[nextBlock] != mBlockThreads[id]) {
		unknownThread(links);
		return;
	}

	bool allThreadsDetermined = true;
	for (const qReal::Id &incomingLink : mRepo.incomingLinks(id)) {
		const qReal::Id &comingFrom = mRepo.otherEntityFromLink(incomingLink, id);
		if (!mBlockThreads.contains(comingFrom)) {
			allThreadsDetermined = false;
			break;
		}
	}

	if (allThreadsDetermined) {
		bool mainThreadFound = false;
		for (const qReal::Id &incomingLink : mRepo.incomingLinks(id)) {
			const qReal::Id &comingFrom = mRepo.otherEntityFromLink(incomingLink, id);
			if (mBlockThreads[comingFrom] == mBlockThreads[id]) {
				mainThreadFound = true;
				break;
			}
		}

		if (!mainThreadFound) {
			error(QObject::tr("Guard property of a link outgoing from a join must contain an id "
					"of one of joined threads"), id);
			return;
		}
	}

	if (mBlockThreads[nextBlock] != mBlockThreads[id]) {
		mBlockThreads[nextBlock] = mBlockThreads[id];
		mSomethingChanged = true;
	}
}

void ThreadsValidator::visitFork(const qReal::Id &id, QList<LinkInfo> &links)
{
	if (!mSecondStage) {
		visitForkFirstStage(id, links);
	} else {
		visitForkSecondStage(id, links);
	}
}

void ThreadsValidator::visitForkFirstStage(const qReal::Id &id, QList<LinkInfo> &links)
{
	if (mBlockThreads[id] == "@@unknown@@") {
		error(QObject::tr("Trying to fork from a thread with an unknown id. Possible causes: "
				"calling fork from a subprogram or trying to merge two threads without a join"), id);
		return;
	}

	if (links.size() < 2) {
		error(QObject::tr("Fork block must have at least TWO outgoing links"), id);
		return;
	}

	for (const LinkInfo &link : links) {
		if (mRepo.stringProperty(link.linkId, "Guard").isEmpty()) {
			error(QObject::tr("All links outgoing from a fork block must have a thread id "
					"specified in a Guard property"), link.linkId);
			return;
		}
	}

	QStringList outgoingThreads;
	for (const LinkInfo &link : links) {
		checkForConnected(link);

		QString const threadId = mRepo.stringProperty(link.linkId, "Guard");
		if (threadId.isEmpty()) {
			error(QObject::tr("All links outgoing from a fork block must have a thread id "
					"specified in a Guard property"), link.linkId);
			return;
		}

		if (outgoingThreads.contains(threadId)) {
			error(QObject::tr("Links outgoing from a fork block must have different thread ids"), id);
			return;
		}

		outgoingThreads << threadId;

		if (mBlockThreads.contains(link.target) && mBlockThreads[link.target] != threadId) {
			mBlockThreads[link.target] = "@@unknown@@";
		} else if (!mBlockThreads.contains(link.target)) {
			mSomethingChanged = true;
			mBlockThreads[link.target] = threadId;
		}
	}

	if (!outgoingThreads.contains(mBlockThreads[id])) {
		error(QObject::tr("Fork block must have a link marked with an id of a thread from which the fork is called, "
				"'%1' in this case").arg(mBlockThreads[id]), id);
	}
}

void ThreadsValidator::visitForkSecondStage(const qReal::Id &id, QList<LinkInfo> &links)
{
	for (const LinkInfo &link : links) {
		const QString threadId = mRepo.stringProperty(link.linkId, "Guard");
		if (threadId != mBlockThreads[id]) {
			if (mThreadCreatingBlocks.contains(threadId) && mThreadCreatingBlocks[threadId] != id) {
				error(QObject::tr("Trying to create a thread with an already occupied id '%1'").arg(threadId), id);
				return;
			}

			mThreadCreatingBlocks[threadId] = id;
		}
	}

	mVisitedBlocks.clear();
	if (achiavable(id, id)) {
		error(QObject::tr("Creation of threads in a cycle is forbidden, check for links to before a fork"), id);
	}
}

bool ThreadsValidator::achiavable(const qReal::Id &id, const qReal::Id &source)
{
	mVisitedBlocks << source;
	for (const qReal::Id &link : mRepo.outgoingLinks(source)) {
		const qReal::Id nextBlock = mRepo.otherEntityFromLink(link, source);
		if ((nextBlock == id) || (!mVisitedBlocks.contains(nextBlock) && achiavable(id, nextBlock))) {
			return true;
		}
	}

	return false;
}

bool ThreadsValidator::foundForks(const qReal::Id &id)
{
	if (id.element() == "Fork" || id.element() == "Join") {
		return true;
	}

	mVisitedBlocks << id;
	for (const qReal::Id &link : mRepo.outgoingLinks(id)) {
		const qReal::Id nextBlock = mRepo.otherEntityFromLink(link, id);
		if (!mVisitedBlocks.contains(nextBlock) && foundForks(nextBlock)) {
			return true;
		}
	}

	return false;
}

void ThreadsValidator::unknownThread(const QList<LinkInfo> &links)
{
	for (const LinkInfo &link : links) {
		if (mBlockThreads[link.target] != "@@unknown@@") {
			mSomethingChanged = true;
			mBlockThreads[link.target] = "@@unknown@@";
		}
	}
}

void ThreadsValidator::error(const QString &message, const qReal::Id &id)
{
	mErrorReporter.addError(message, id);
	mNoErrors = false;
}

void ThreadsValidator::checkForConnected(LinkInfo const &link)
{
	if (!link.connected) {
		error(QObject::tr("Outgoing link is not connected"), link.linkId);
	}
}
