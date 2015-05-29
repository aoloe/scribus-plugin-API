#include "scribusAPI.h"
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
    /*
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ExportAsEPUB";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Save as &EPUB...");
	// Menu
	m_actionInfo.menu = "FileExport";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
    */
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

/*
bool ScribusAPIPlugin::run(ScribusDoc* doc, QString target)
{
    return true;
}
*/
