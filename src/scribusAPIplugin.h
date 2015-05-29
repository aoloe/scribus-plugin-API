 /**
 * This is the entry point for the plugin.
 * All the methods defined here are called by Scribus' plugin execution code.
 */

#ifndef SCRIBUSAPIPLUGIN_H
#define SCRIBUSAPIPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"

class QString;
class ScribusDoc;
class ScribusMainWindow;

class PLUGIN_API ScribusAPIPlugin : public ScPlugin
{
	Q_OBJECT
public:
	ScribusAPIPlugin();
	virtual ~ScribusAPIPlugin();
	/*!
	\author Ale Rimoldi
	\brief Run the EPUB export
	\param filename a file to export to
	\retval bool true
	*/
	/* virtual bool run(ScribusDoc* doc=0, QString filename = QString::null); */
	virtual const QString fullTrName() const;
	virtual const AboutData* getAboutData() const;
	virtual void deleteAboutData(const AboutData* about) const;
	virtual void languageChange();
	virtual void addToMainWindowMenu(ScribusMainWindow *) {};
};

extern "C" PLUGIN_API int scribusAPI_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribusAPI_getPlugin();
extern "C" PLUGIN_API void scribusAPI_freePlugin(ScPlugin* plugin);

#endif // SCRIBUSAPIPLUGIN_H
