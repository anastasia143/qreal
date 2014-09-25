#include "databasesSupportPlugin.h"

namespace qReal {
namespace databasesSupportPlugin {

DatabasesSupportPlugin::DatabasesSupportPlugin(QObject *parent) :
	QObject(parent)
{
}


void DatabasesSupportPlugin::init(PluginConfigurator const &configurator)
{
	//mVisualDebugSupport = new VisualDebugSupport(configurator);
	//initActions();
}

}
}
