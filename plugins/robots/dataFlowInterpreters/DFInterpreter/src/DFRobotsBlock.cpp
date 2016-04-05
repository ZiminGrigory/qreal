#include "DFRobotsBlock.h"



using namespace dataFlow::interpretation;
using namespace qReal;

void DFRobotsBlock::configure()
{
	if (property(mGraphicalId, "synch").toBool()) {
		qReal::IdList localIncomingLinks = mGraphicalModelApi->graphicalRepoApi().incomingLinks(mGraphicalId);
		for (auto &id : localIncomingLinks) {
			synchronisedPorts.insert(qRound64(mGraphicalModelApi->toPort(id)));
		}
		int cFInPort = portAssociatedWithProperty.value("CF_IN", -1);
		if(cFInPort != -1) {
			synchronisedPorts.remove(cFInPort);
		}
	}
}

const qReal::Id DFRobotsBlock::id() const
{
	return mGraphicalId;
}

DFRobotsBlock::DFRobotsBlock()
{
}

QVariant DFRobotsBlock::property(int portNumber)
{
	return valueOnPort.value(portNumber);
}

QVariant DFRobotsBlock::propertyFromPort(const QString& name)
{
	return property(portAssociatedWithProperty.value(name));
}


void DFRobotsBlock::init(const qReal::Id &graphicalId
		, const qReal::GraphicalModelAssistInterface &graphicalModelApi
		, const qReal::LogicalModelAssistInterface &logicalModelApi
		, qReal::ErrorReporterInterface * const errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguageToolbox
		)
{
	mGraphicalId = graphicalId;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = errorReporter;
	mParser = &textLanguageToolbox;
	if (mLogicalModelApi) {
		mParserErrorReporter.reset(new utils::ParserErrorReporter(*mParser, *mErrorReporter
				, mLogicalModelApi->editorManagerInterface()));
	}

	mRobotModelManager = &robotModelManager;
}

void dataFlow::interpretation::DFRobotsBlock::handleNewDataFromFlow(const QVariant &data, int port)
{
	valueOnPort[port] = data;
	synchronisedPorts.remove(port);
	if (synchronisedPorts.empty()) {
		handleData();
		configure();
	}
}


QVariant DFRobotsBlock::property(const QString &propertyName)
{
	return property(id(), propertyName);
}

QString DFRobotsBlock::stringProperty(const QString &propertyName)
{
	return stringProperty(id(), propertyName);
}

int DFRobotsBlock::intProperty(const QString &propertyName)
{
	return intProperty(id(), propertyName);
}

bool DFRobotsBlock::boolProperty(const QString &propertyName)
{
	return boolProperty(id(), propertyName);
}

QVariant DFRobotsBlock::property(const Id &id, const QString &propertyName)
{
	const Id logicalId = mGraphicalModelApi->logicalId(id);
	if (logicalId.isNull()) {
		// If we get here we definitely have such situation:
		// graphical id existed when this Block instance was constructed (or we just will not get here),
		// but now the logical instance has suddenly disppeared.
		error(tr("Block has disappeared!"));
		return QVariant();
	}

	return mLogicalModelApi->propertyByRoleName(logicalId, propertyName);
}

QString DFRobotsBlock::stringProperty(const Id &id, const QString &propertyName)
{
	return property(id, propertyName).toString();
}

int DFRobotsBlock::intProperty(const Id &id, const QString &propertyName)
{
	return property(id, propertyName).toInt();
}

bool DFRobotsBlock::boolProperty(const Id &id, const QString &propertyName)
{
	return property(id, propertyName).toBool();
}

QColor DFRobotsBlock::propertyToColor(const QString &property) const
{
	// Qt does not support dark-yellow string color (see QColor::colorNames())
	return property == "darkYellow" ? QColor(Qt::darkYellow) : QColor(property);
}

void DFRobotsBlock::error(const QString &message)
{
	mErrorReporter->addError(message, id());
	emit failure();
}

void DFRobotsBlock::warning(const QString &message)
{
	mErrorReporter->addWarning(message, id());
}

void DFRobotsBlock::evalCode(const QString &code)
{
	evalCode<int>(code);
}

void DFRobotsBlock::eval(const QString &propertyName)
{
	eval<int>(propertyName);
}

bool DFRobotsBlock::errorsOccured() const
{
	return !mParser->errors().isEmpty();
}
