/**
 * ScribusAPIDocumentItem is a proxy to the scribus' PageItem and providing access to the
 * items / frames.
 */
/*
 * This file is part of the ScribusAPIDocument plugin
 * (c) GPL 2, Ale Rimoldi <ale@graphicslab.org>
 * For the full copyright and license information, please view the LICENSE
 */
#ifndef SCRIBUSAPIDOCUMENTITEM_H
#define SCRIBUSAPIDOCUMENTITEM_H

#include <QDebug>
#include <QObject>

#include <QVector>
#include <QSize>

#include "scribusdoc.h"

class ScribusAPIDocumentItemText;

struct ScribusAPIDocumentItemResourceFile
{
    QString filename;
    QString filePath;
    QString fileContent;
};
struct ScribusAPIDocumentItemImageWeb
{
    QString path;
    QString filename;
    QString content;
    QSize imageSize;
    QString description; // for alt text
    QString mediatype;
};

class ScribusAPIDocumentItem : public QObject
{
    Q_OBJECT
public:
	ScribusAPIDocumentItem(PageItem* item) : scribusItem{item} {}
	~ScribusAPIDocumentItem() {}
    PageItem* getItem() { return scribusItem; }
    void setPageNumber(int pageNumber) { pageNumber = pageNumber; }
    bool isBefore(ScribusAPIDocumentItem* const item) const;
    /**
     * used by qSort to sort the items by their place on the page
     */
    static bool isBeforeQSort(ScribusAPIDocumentItem* const item1, ScribusAPIDocumentItem* const item2) { /*return true; } // */ return item1->isBefore(item2); }
    int getX() const { return scribusItem->gXpos; }
    int getY() const { return scribusItem->gYpos; }
    void render();

    bool isTextFrame() { return scribusItem->asTextFrame(); }
    bool isImageFrame() { return scribusItem->asImageFrame(); }
    ScribusAPIDocumentItemText* getText();

    QString getName() { return scribusItem->itemName(); }


    ScribusAPIDocumentItemImageWeb getImageWeb();
    QList<ScribusAPIDocumentItemResourceFile> getResourceFiles();

protected:
    int pageNumber;
    PageItem* scribusItem;
};

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItem &scribusDocumentItem);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemResourceFile file);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemImageWeb image);

#endif //SCRIBUSAPIDOCUMENTITEM_H 
