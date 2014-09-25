#pragma once

#include <QtCore/QTranslator>
#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

namespace qReal {
namespace databasesSupportPlugin {

class DatabasesSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.databasesSupport.DatabasesSupportPlugin")
public:
	DatabasesSupportPlugin(QObject *parent = 0);
	virtual void init(qReal::PluginConfigurator const &configurator);
signals:

public slots:

};

}
}
