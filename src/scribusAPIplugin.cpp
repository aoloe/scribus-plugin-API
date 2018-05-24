// #include "scribusAPI.h"
#include "scribusAPIplugin.h"

#include "scribuscore.h"
#include "scribusdoc.h"

int scribusAPI_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribusAPI_getPlugin()
{
	ScribusAPIPlugin* plug = new ScribusAPIPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribusAPI_freePlugin(ScPlugin* plugin)
{
	ScribusAPIPlugin* plug = dynamic_cast<ScribusAPIPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ScribusAPIPlugin::ScribusAPIPlugin() : ScPlugin()
{
}

ScribusAPIPlugin::~ScribusAPIPlugin()
{
	// unregisterAll();
};

void ScribusAPIPlugin::languageChange()
{
}

const QString ScribusAPIPlugin::fullTrName() const
{
	return QObject::tr("Scribus API");
}

const ScActionPlugin::AboutData* ScribusAPIPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Ale Rimoldi <a.l.e@ideale.ch>";
	about->shortDescription = tr("Scribus API");
	about->description = tr("Provides a stable, documented and user friendlay API\nto the Scribus internals.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ScribusAPIPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}
