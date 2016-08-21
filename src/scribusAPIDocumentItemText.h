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

#include "scribusAPIDocumentItem.h"
class ScribusAPIDocumentItemTextFormatting;

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

	bool isFirstInChain() { return documentItem->getItem()->prevInChain() == NULL; }
	bool isEmpty() { return documentItem->getItem()->itemText.length() == 0; }

	QVector<ScribusAPIDocumentItemTextRuns> getStoryRuns();
	QVector<ScribusAPIDocumentItemTextRuns> getRuns();
	QVector<ScribusAPIDocumentItemTextRuns> getRuns(int start, int end);
	QList<QDomElement> getDom(QDomDocument xhtmlDocument);

	ScribusAPIDocumentItemTextFormatting getFormattingAtPosition(int position);

	int getLength();
	int getStoryLength();
	void setCursorPosition(int position);
	void insert(const QString text, const QString paragraphStyle = "", const QString characterStyle = "");
	void append(const QString text, const QString paragraphStyle = "", const QString characterStyle = "");
	void appendParagraph(const QString text, const QString paragraphStyle = "");
	void applyParagraphStyle(const QString styleName);
	void applyParagraphStyle(int position, const QString styleName);

private:
	ScribusAPIDocumentItem* documentItem;
	QString getStylenameSanitized(QString stylename);
};

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemText &item);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemTextRuns run);
QDebug operator<<(QDebug dbg, const QVector<ScribusAPIDocumentItemTextRuns> &runs);

#endif //SCRIBUSAPIDOCUMENTITEMTEXT_H 
