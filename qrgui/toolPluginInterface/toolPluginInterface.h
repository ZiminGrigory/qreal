#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QList>

#include "../../qrrepo/repoControlInterface.h"

#include "interpreterInterface.h"
#include "customToolInterface.h"
#include "customizationInterface.h"
#include "pluginConfigurator.h"

namespace qReal {

class ToolPluginInterface
{
public:
	virtual void initPlugin(PluginConfigurator const &configurator) = 0;

	virtual QList<CustomToolInterface *> toolInterfaces() {
		return QList<CustomToolInterface *>();
	}

	virtual QList<InterpreterInterface *> interpreterInterfaces() {
		return QList<InterpreterInterface *>();
	}

	virtual QList<CustomizationInterface *> customizationInterfaces() {
		return QList<CustomizationInterface *>();
	}
};

}

Q_DECLARE_INTERFACE(qReal::ToolPluginInterface, "ru.tepkom.QReal.ToolPluginInterface/0.1")