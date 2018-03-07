/**
 * ScribusAPIDocument contains a reference to the current scribus document and is used as
 * an interface to get information and parse the scribus document itself
 */
#ifndef SCRIBUSAPIDOCUMENT_H
#define SCRIBUSAPIDOCUMENT_H

#include <QDebug>

#include <QObject>

#include <QString>
#include <QList>
#include <QStringList>
#include <QVector>

#include "scribusdoc.h"

#include "scribusAPIDocumentItem.h"

// class ScribusDoc;
// class PageItem;
class ScPage; // for getPageRect() remove it, it's moved to ScPage
class MarginStruct; // for getPageRect() remove it, it's moved to ScPage
class PageItem;

class ScribusAPIDocumentMetadata;
class ScribusAPIDocumentItem;

struct ScribusAPIDocumentMetadata
{
    QString title;
    QString language;
    QString id;
    QString date;
    QString author;
	// non mandatory fields from the main screen
    QString subject;
    QString keywords;
	QString description;
	QString publisher;
	// non mandatory fields from the further information screen
	QString contributor;
	QString type;
    QString format;
	QString source;
	QString relation;
	QString coverage;
	QString rights;
    ScribusAPIDocumentMetadata():
        title(""), language(""), id(""), date(""), author(""),
        subject(""), keywords(""), description(""), publisher(""),
        contributor(""), type(""), format(""), source(""), relation(""), coverage(""), rights("")
    {}
};

class ScribusAPIDocument : public QObject
{
    Q_OBJECT

public:
	ScribusAPIDocument(ScribusDoc* scribusDoc) : scribusDoc{scribusDoc} {};
	ScribusAPIDocument();
	~ScribusAPIDocument();

    /**
     * @deprecated Construct one ScribusAPIDocument object for each scribusDoc.
     */
    void set(ScribusDoc* scribusDoc) { this->scribusDoc = scribusDoc; }
    void setPageRange(QList<int> pageRange) { this->pageRange = pageRange; }

    QString getPath() { return scribusDoc->DocName; }
    ScribusAPIDocumentMetadata getMetadata();
    bool isPortrait(int pageNumber);
    void render();

    void readItems();
    QList<ScribusAPIDocumentItem*> getPageItems(int page) { return this->items[page];}
    // QList<ScribusAPIDocumentItem*> getPageItems(int page) { QList<ScribusAPIDocumentItem*> result; return result;}
	ScribusAPIDocumentItem* getCurrentItem();

    void readSections();
    QVector< QList<int> > getSections() { return this->sections; }
    QString getStylesAsCss();
    QStringList getParagraphStyleNames();
    QStringList getCharacterStyleNames();
    QByteArray getPageAsPNG(int pageNumber, int width, int height);
    QByteArray getPageAsPNG(int pageNumber, int maxSize);
    QByteArray getFirstPageAsCoverImage();

    int getPageCount() { return this->scribusDoc->DocPages.count(); }
	QString getPageNumberByIndex(int i);
    int getSectionCount() { return this->scribusDoc->prefsData().docSectionMap.count(); }
private:
    ScribusDoc* scribusDoc;

    QList<int> pageRange; // from the options

    QVector< QList<ScribusAPIDocumentItem*> > items;
    QVector< QList<int> > sections;

    QString getStylenameSanitized(QString stylename);


    static bool isDocItemTopLeftLessThan(const PageItem *docItem1, const PageItem *docItem2);
    static bool isItemBefore(const ScribusAPIDocumentItem& item1, const ScribusAPIDocumentItem& item2);

    // ==== functions to be moved to scribusDoc? =====
    QList<ScPage *> getPagesWithItem(PageItem* item);
    QRect getPageRect(const ScPage* page);
    MarginStruct getPageBleeds(const ScPage* page);
    QList<ScPage*> getPagesList();

};

QDebug operator<<(QDebug dbg, const ScribusAPIDocument &scribusDocument);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentMetadata &metadata);

#endif // SCRIBUSAPIDOCUMENT_H
