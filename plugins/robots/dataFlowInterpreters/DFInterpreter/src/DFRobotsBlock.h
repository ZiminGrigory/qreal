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


#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtGui/QColor>
#include <QtCore/QObject>
#include <QtCore/QQueue>

#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrtext/languageToolboxInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <qrutils/parserErrorReporter.h>


#include "DFRobotsBlockInterface.h"

namespace dataFlow {
namespace interpretation {

/// Base class for all blocks implementations used in robots interpreter.
class DFRobotsBlock : public DFRobotsBlockInterface
{
	Q_OBJECT

public:
	virtual ~DFRobotsBlock(){}

	void init(const qReal::Id &graphicalId
			, const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);


	void handleNewDataFromFlow(const QVariant &data, int port) override;

	virtual bool isSynchronizationNeeded();

	/// We need synchronize some ports of DFBlock or etc.
	virtual void configureSynchronizedPorts();

	const qReal::Id id() const override;

	int activationPortNumber() const override;

	int getPortAssociatedWithProperty(const QString &propertyName);

protected:
	DFRobotsBlock();
	virtual void init(){}

	/// Enum with flags whether to report parser errors or let the language toolbox fail silently.
	enum class ReportErrors {
		/// Do report parser errors.
		report

		/// Do not report errors to user. Used when parser shall only try to parse a code (for example, in
		/// autoconfigurer).
		, doNotReport
	};


	/// check for existing property associated with given port.
	bool hasNewData(int portNumber);

	/// check for existing property associated with given name which associated with some port.
	bool hasNewData(const QString &propertyName);

	/// returns a property associated with given port.
	QVariant property(int portNumber);

	/// returns a property associated with given name which associated with some port.
	QVariant propertyFromPort(const QString &propertyName);

	/// Returns a property of current block with given name as QVariant.
	QVariant property(const QString &propertyName);

	/// Returns a property of current block with given name as QString.
	QString stringProperty(const QString &propertyName);

	/// Returns a property of current block with given name as int, or 0 if it can't be converted to int.
	int intProperty(const QString &propertyName);

	/// Returns a property of current block with given name as bool, or "false" if it can't be converted to bool.
	bool boolProperty(const QString &propertyName);

	/// Returns a property with given name of block with given id as QVariant.
	QVariant property(const qReal::Id &id, const QString &propertyName);

	/// Returns a property with given name of block with given id as QString.
	QString stringProperty(const qReal::Id &id, const QString &propertyName);

	/// Returns a property with given name of block with given id as int, or 0 if it can't be converted to int.
	int intProperty(const qReal::Id &id, const QString &propertyName);

	/// Returns a property with given name of block with given id as bool, or "false" if it can't be converted to bool.
	bool boolProperty(const qReal::Id &id, const QString &propertyName);

	/// Returns a property of current block with given name as color.
	QColor propertyToColor(const QString &property) const;

	/// Reports error and emits "failure" signal.
	void error(const QString &message);

	/// Reports warning.
	void warning(const QString &message);

	/// Evaluates contents of a given property using text language interpreter and returns result.
	template<typename T>
	T eval(const QString &propertyName, ReportErrors reportErrors = ReportErrors::report)
	{
		return evalCode<T>(stringProperty(propertyName), propertyName, reportErrors);
	}

	/// Evaluates given code using text language interpreter and returns result.
	template<typename T>
	T evalCode(const QString &code, ReportErrors reportErrors = ReportErrors::report)
	{
		return evalCode<T>(code, "", reportErrors);
	}

	/// Evaluates given code using text language interpreter and returns result.
	/// @param code - code to evaluate.
	/// @param propertyName - name of corresponding property, used for connection.
	template<typename T>
	T evalCode(const QString &code, const QString &propertyName, ReportErrors reportErrors = ReportErrors::report)
	{
		T result = mParser->interpret<T>(mGraphicalId, propertyName, code);
		if (!mParser->errors().isEmpty() && reportErrors == ReportErrors::report) {
			if (!mParserErrorReporter.isNull()) {
				mParserErrorReporter->reportErrors(id(), propertyName);
			}

			emit failure();
			return result;
		}

		return result;
	}

	/// Parse list to {<..>,...,<..>} structure
	QString qVariantListToLuaArrayInitializeList(const QVariantList &list);

	/// Convert QVector<int> to QVariantList structure
	QVariantList vectorToQVariantList(const QVector<int> &vector);

	void setVariable(const QString &varName, QVariant value);

	/// Evaluates given code using text language interpreter.
	void evalCode(const QString &code);

	/// Evaluates contents of a given property using text language interpreter.
	void eval(const QString &propertyName);

	/// Returns true, if there were parser/interpreter errors on last eval() call.
	bool errorsOccured() const;

	/// @todo: there is no such things as protected fields. State of a class shall not be directly available to
	/// descendants.

	/// Shall be reimplemented to provide semantics of block execution.
	virtual void handleData() = 0;


	QMap<int, QQueue<QVariant>> valueOnPort;
	QMap<QString, int> portAssociatedWithProperty;
	QSet<int> synchronisedPorts;

	qReal::Id mGraphicalId;
	const qReal::GraphicalModelAssistInterface *mGraphicalModelApi = nullptr;  // Doesn't have ownership.
	const qReal::LogicalModelAssistInterface *mLogicalModelApi = nullptr;  // Doesn't have ownership.
	const kitBase::robotModel::RobotModelManagerInterface *mRobotModelManager = nullptr;  // Doesn't have ownership.

private:
	qrtext::LanguageToolboxInterface *mParser = nullptr;  // Doesn't have ownership.

	qReal::ErrorReporterInterface *mErrorReporter = nullptr;  // Doesn't have ownership.

	/// @todo: Block shall not own ParserErrorReporter, it shall be received from factory.
	QScopedPointer<utils::ParserErrorReporter> mParserErrorReporter;
};

}
}

