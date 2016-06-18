/**
 * @brief ScribusAPIDocumentItemText provide acess to text frames.
 *
 */
/*
 * This file is part of the ScribusAPIDocument plugin
 * (c) GPL 2, Ale Rimoldi <ale@graphicslab.org>
 * For the full copyright and license information, please view the LICENSE
 */
#ifndef SCRIBUSAPIDOCUMENTITEMTEXT_H
#define SCRIBUSAPIDOCUMENTITEMTEXT_H

#include <QDebug>
#include <QObject>
#include <QVector>
#include <QDomDocument>

class QString;

class ScribusAPIDocumentItem;
#include "scribusAPIDocumentItem.h"

struct ScribusAPIDocumentItemTextRuns
{
	int position;
	int length;
	char type; // p=paragraph, f=formatting
	QVector< QVector<QString> > content;
	bool isParagraph() {return type == 'p';}
};

class ScribusAPIDocumentItemText : public QObject
{
	Q_OBJECT
public:
	ScribusAPIDocumentItemText(ScribusAPIDocumentItem* documentItem);
	~ScribusAPIDocumentItemText();

	bool isTextFirstInChain() { return documentItem->getItem()->prevInChain() == NULL; }
	bool isTextEmpty() { return documentItem->getItem()->itemText.length() == 0; }

	QVector<ScribusAPIDocumentItemTextRuns> getTextRuns();
	QList<QDomElement> getTextDom(QDomDocument xhtmlDocument);

	ScribusAPIDocumentItemFormatting getFormattingAtPosition(int position);

private:
	ScribusAPIDocumentItem* documentItem;
	QString getStylenameSanitized(QString stylename);
};

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemText &item);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemTextRuns run);
QDebug operator<<(QDebug dbg, const QVector<ScribusAPIDocumentItemTextRuns> &runs);

#endif //SCRIBUSAPIDOCUMENTITEMTEXT_H 
