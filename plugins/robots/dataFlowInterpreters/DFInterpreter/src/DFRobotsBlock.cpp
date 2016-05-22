#include "DFRobotsBlock.h"



using namespace dataFlow::interpretation;
using namespace qReal;

void DFRobotsBlock::configureSynchronizedPorts()
{
	if (isSynchronizationNeeded()) {
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

int DFRobotsBlock::activationPortNumber() const
{
	return portAssociatedWithProperty.value("CF_IN", -1);
}

int DFRobotsBlock::getPortAssociatedWithProperty(const QString &propertyName)
{
	return portAssociatedWithProperty[propertyName];
}

DFRobotsBlock::DFRobotsBlock()
{
}

bool DFRobotsBlock::hasNewData(int portNumber)
{
	return !valueOnPort.value(portNumber).isEmpty();
}

bool DFRobotsBlock::hasNewData(const QString &propertyName)
{
	return hasNewData(portAssociatedWithProperty[propertyName]);
}

QVariant DFRobotsBlock::property(int portNumber)
{
	QQueue<QVariant> &localValue = valueOnPort[portNumber];
	if (localValue.isEmpty()) {
		return QVariant();
	}

	return localValue.dequeue();
}

QVariant DFRobotsBlock::propertyFromPort(const QString& propertyName)
{
	return property(portAssociatedWithProperty.value(propertyName));
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

	init();
}

void dataFlow::interpretation::DFRobotsBlock::handleNewDataFromFlow(const QVariant &data, int port)
{
	valueOnPort[port].enqueue(data);
	synchronisedPorts.remove(port);
	if (synchronisedPorts.empty()) {
		// sequence is important, because handleDare method can stopping interpretation
		configureSynchronizedPorts();
		handleData();
	}
}

bool DFRobotsBlock::isSynchronizationNeeded()
{
	return false;
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

QString DFRobotsBlock::qVariantListToLuaArrayInitializeList(const QVariantList &list)
{
	QString res("{");
	for (const QVariant &elem : list) {
		QString var = elem.toString();
		//{data} -> data
		var.remove(0,1);
		var.chop(1);
		res += var != "" ? (var + ",") : "";
	}

	res += "}";
	res.replace(",}", "}");

	return res;
}

QVariantList DFRobotsBlock::vectorToQVariantList(const QVector<int> &vector)
{
	QVariantList res;
	for (const int a : vector) {
		res.append(QVariant(a));
	}

	return res;
}

void DFRobotsBlock::setVariable(const QString &varName, QVariant value)
{
	if (value.canConvert<QVariantList>()) {
		mParser->setVectorVariableValue(varName, value.value<QVariantList>());
	} else {
		mParser->setVariableValue(varName, value);
	}
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
