#include "robotsPlugin.h"

Q_EXPORT_PLUGIN2(robotsPlugin, qReal::interpreters::robots::RobotsPlugin)

using namespace qReal::interpreters::robots;

RobotsPlugin::RobotsPlugin()
{
}

RobotsPlugin::~RobotsPlugin()
{
}

void RobotsPlugin::initPlugin(PluginConfigurator const &configurator)
{
	mInterpreter.init(configurator.graphicalModelApi()
			, configurator.logicalModelApi()
			, configurator.mainWindowInterpretersInterface());
}

QList<qReal::CustomToolInterface *> RobotsPlugin::toolInterfaces()
{
	return QList<qReal::CustomToolInterface *>();
}

QList<qReal::InterpreterInterface *> RobotsPlugin::interpreterInterfaces()
{
	return QList<qReal::InterpreterInterface *>() << &mInterpreter;
}