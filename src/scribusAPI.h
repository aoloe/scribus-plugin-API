/**
 * Not in use, yet. Might be removed if no use for it is found (defining the version of the API?)
 */
#ifndef SCRIBUSAPI_H
#define SCRIBUSAPI_H

#include <QObject>

class ScribusDoc;

class ScribusAPI : public QObject
{
    Q_OBJECT

public:
	ScribusAPI();
	~ScribusAPI();
};

#endif // SCRIBUSAPI_H
