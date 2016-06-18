#include "scribusAPIDocumentItemText.h"

#include <QDebug>
#include <QVector>
#include <QList>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include "scribusAPIDocumentItem.h"
#include "scribusAPIDocumentItemFormatting.h"

ScribusAPIDocumentItemText::ScribusAPIDocumentItemText(ScribusAPIDocumentItem* documentItem) : documentItem{documentItem}
{
}

ScribusAPIDocumentItemText::~ScribusAPIDocumentItemText()
{
}

/**
 * @brief Parse the text and define the runs list.
 *
 * Runs are strings with a common formatting. The paragraph ends the run.
 *
 * this method is based on pierre's work for the mitical OIF branch
 * ... also inspired by Scribus150Format::writeITEXTs
 * @todo:
 * - use the text/storytext methods as soon as they are implemented
 */
QVector<ScribusAPIDocumentItemTextRuns> ScribusAPIDocumentItemText::getTextRuns()
{
	QVector<ScribusAPIDocumentItemTextRuns> runs; // the indexes where the runs start
	struct ScribusAPIDocumentItemTextRuns runsItem;
	runs.clear();

	CharStyle lastStyle;
	int lastPos = 0;
	runsItem.position = 0;
	runsItem.type = 'p';
	runsItem.content.clear();

	StoryText itemText = documentItem->getItem()->itemText;
	int n = itemText.length();

	QVector<QString> lines;
	QString line = "";

	if (n == 0) {
		runsItem.length = 0;
		runs.append(runsItem);
	} else {
		lines.clear();
		QString content = itemText.text(0, itemText.length());

		for(int i = 0; i < n; ++i)
		{
			bool output = true;

			const CharStyle& style1(itemText.charStyle(i));
			const QChar ch = itemText.text(i);
			if (itemText.hasMark(i))
			{
				Mark* mark = itemText.mark(i);
				// qDebug() << "properties mark " << mark->getType(); // 4 is a footnote
				// qDebug() << "MARKNoteFrameType " << MARKNoteFrameType; // 5
				// qDebug() << "MARKNoteMasterType" << MARKNoteMasterType; // 4
				if (mark->getType() == MARKNoteMasterType) // 4
				{
					TextNote* footnote = mark->getData().notePtr;
					qDebug() << "calling mark:" << mark->getString();
					if (!footnote->saxedText().isEmpty())
					{
						qDebug() << "footnote text?" << footnote->saxedText();
						// TODO: desaxe needs a reference to scribusDoc which we dont have.
						// but saveText() looks like this:
						// <?xml version=\"1.0\" encoding=\"UTF-8\"?><SCRIBUSTEXT ><defaultstyle /><p ><style LineSpacingMode=\"1\" /><span ><charstyle Features=\"inherit \" />this is </span><span ><charstyle Features=\"inherit \" FontSize=\"80\" />the</span><span ><charstyle Features=\"inherit \" /> footnote</span></p></SCRIBUSTEXT>\n"
						// we should probably use the Qt XML parser to extract the text and formatting. or read
						// andreas post http://lists.scribus.info/pipermail/scribus-dev/2014-May/002084.html and
						// decide...



						// StoryText footnoteText = desaxeString(scribusDoc, footnote->saxedText());
						// qDebug() << "note text:" << footnoteText.text(0, footnoteText.length());
						// TODO: refactor to be able to recursively call the html formatting!
					}
				}
				if (mark->getType() == MARKNoteFrameType) // 5
				{
					continue;
					// ignore: we already collected the footnote text, when the master mark has been found
				}
			}
			/*
			// TODO: we have to correctly handle the marks
			ScText* chProperties = itemText.item(i);
			if (chProperties->mark) {
			Mark* footnoteCall = chProperties->mark;
			if (footnoteCall->getType() == MARKNoteMasterType) {
			TextNote* footnote = chProperties->mark->getData().notePtr;
			qDebug() << "calling mark:" << footnoteCall->getString();
			if (!footnote->saxedText().isEmpty()) {
			StoryText footnoteText = desaxeString(doc, footnote->saxedText());
			qDebug() << "note text:" << footnoteText.text(0, footnoteText.length());
			// TODO: refactor to be able to recursively call the html formatting!
			}
			qDebug() << "note text:" << footnote->saxedText();
			qDebug() << "note mark:" << footnote->num();
			qDebug() << "note style:" << footnote->notesStyle()->name();

			// apptend 
			} else if (footnoteCall->getType() == MARKNoteFrameType) {
			}
			continue; // don't insert the marks as such!
			}
			 */

			/*
			// store formatting struct for:
			// - current paragraph style
			// - current character style
			// - sum of current paragraph and char style
			// - currentformatting
			// store last insterted formatting string (<strong><em><span style="font-size:12px");
			// --> struct EPUBExportCurrentFormatting
			 */

			// if there is a paragraph change or the formatting has changed
			if (
					ch == SpecialChars::PARSEP ||
					ch == SpecialChars::COLBREAK ||
					ch == SpecialChars::FRAMEBREAK
			   )
			{
				// qDebug() << "ch" << ch.unicode();
				runsItem.length = i - runsItem.position;
				if (line != "")
				{
					lines.append(line);
					line = "";
				}
				if (!lines.isEmpty())
				{
					runsItem.content.append(lines);
					lines.clear();
				}
				if (!runsItem.content.isEmpty())
					runs.append(runsItem);
				runsItem.position = i;
				runsItem.type = 'p';
				runsItem.content.clear();
				output = false;
			}
			if (ch == SpecialChars::LINEBREAK)
			{
				if (line != "")
				{
					lines.append(line);
					line = "";
				}
				output = false;
			}
			// accepted special chars:
			// ch == SpecialChars::NBSPACE

			if (
					style1 != lastStyle
			   )
			{
				// something new, write pending chars
				if  (i - lastPos > 0)
				{
					// docu.writeEmptyElement("ITEXT");
					/*if (item->getItem()->asPathText()) // seems to cause problems when loading pathtext elements
					  putCStylePT(docu, lastStyle);
					  else*/
					// putCStyle(docu, lastStyle);
					// docu.writeAttribute("CH", textWithSoftHyphens(item->getItem()->itemText, lastPos, i));
				}
				// runs.append(i);
				if (line != "")
				{
					lines.append(line);
					line = "";
				}
				if (!lines.isEmpty())
				{
					runsItem.content.append(lines);
					lines.clear();
				}
				runsItem.length = i - runsItem.position;
				if (runsItem.type == 'p' || !runsItem.content.isEmpty())
					runs.append(runsItem);
				runsItem.position = i;
				runsItem.type = 'f';
				runsItem.content.clear();
				lastStyle = style1;
				lastPos = i;
			}
			if (
					ch == SpecialChars::NBHYPHEN ||
					ch == SpecialChars::NBSPACE ||
					ch == SpecialChars::ZWNBSPACE ||
					ch == SpecialChars::ZWSPACE
			   )
			{
			}
			// @todo: ignore the following chars
			if (
					ch == SpecialChars::OBJECT ||
					ch == SpecialChars::TAB ||
					ch == SpecialChars::PAGENUMBER ||
					ch == SpecialChars::PAGECOUNT ||
					ch.unicode() < 32 || 
					(0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
					ch.unicode() == 0xfffe || ch.unicode() == 0xffff
			   )
			{
			}

			if (output)
				line += ch;
		}
		runsItem.length = n - runsItem.position;
		if (line != "")
			lines.append(line);

		if (!lines.isEmpty())
			runsItem.content.append(lines);

		if (!runsItem.content.isEmpty())
			runs.append(runsItem);
	}
	// qDebug() << "runs: " << runs;
	// qDebug() << "paragraph: " << paragraph;
	return runs;
}

/**
 * example of charStyle() use in svgexplugin.cpp 
 * cf. short-words/parse.cpp
 * cf. Scribus150Format::writeITEXTs() for getting the local formatting
 * in plugins/fileloader/scribus150format/scribus150format_save.cpp
 *
 * - get the list of runs (chunks different formatting) in the text frame
 * - foreach run:
 *   - if it's a p
 *     - create a p element with it's style name
 *     - set this element as the latest inserted paragraph
 *     - set the the dom element as the current one
 *   - get the character style
 *   - get the local formatting of the first char in the run
 *     - font name
 *       - if the the font name suggests an italic or bold add italic or bold to the list of the formatting
 *   - foreach formatting (feature)
 *     - if it's bold, italic, superscript, subscript create an element with the specfic tag
 *     - if it's underline, strikethrough add the formatting to the span's style attribute
 *   - if there is a character style of a local formatting:
 *     - if not specific tag has been createdi create a span element
 *     - add the class and style attributes to the element
 *     - add the span to the paragraph element and set it as the current element
 *   - insert the text line by line (with a br at the end of each but the last line)
 *   - add the p to the dom
 *   - set the current element as the latest element paragraph created
 *   @todo:
 *   - should xhtmlDocument be passed by value?
 */
QList<QDomElement> ScribusAPIDocumentItemText::getTextDom(QDomDocument xhtmlDocument)
{
    QList<QDomElement> result;
    // initialize the local variables
    QDomElement elementCurrent;
    QDomElement elementParagraph;
    QDomElement element;
    QString paragraphStyleName;
    QString characterStyleName;
    QString run_text;

    // qDebug() << "item name" << item->itemName();
    ScribusAPIDocumentItemFormatting formatting;
    formatting.setItem(documentItem->getItem());
    // qDebug() << "isTextFirstInChain" << isTextFirstInChain(); 
    // qDebug() << "isTextEmpty" << isTextEmpty(); 

    if (isTextFirstInChain() && (!isTextEmpty()))
    {
        foreach (ScribusAPIDocumentItemTextRuns run, getTextRuns())
        {
            formatting.readAtPosition(run.position);

            // qDebug() << "run:" << run;
            if (run.isParagraph())
            {
                if (!elementParagraph.isNull())
                    result.append(elementParagraph);

                elementParagraph = xhtmlDocument.createElement("p");

                paragraphStyleName = formatting.getParagraphStyleName();
                paragraphStyleName = getStylenameSanitized(paragraphStyleName);
                // qDebug() << "paragraphStyle:" << paragraphStyleName;
                if (paragraphStyleName != "")
                    elementParagraph.setAttribute("class", paragraphStyleName);

                elementCurrent = elementParagraph;
            }

            QStringList characterFormatting;

            if (formatting.isBold())
            {
                qDebug() << "paragraphStyleName" << paragraphStyleName;
                // TODO: only set b (and i & co) if b is not set in the paragraph or character style
                // TODO: find a way to remove the bold if it's in the style but not in the local formatting
                element = xhtmlDocument.createElement("b");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isItalic())
            {
                element = xhtmlDocument.createElement("i");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isSuperscript())
            {
                element = xhtmlDocument.createElement("sup");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isSubscript())
            {
                element = xhtmlDocument.createElement("sub");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isUnderline() || formatting.isUnderlineWords())
            {
                characterFormatting << "text-decoration:underline";
            }
            if (formatting.isStrikethrough())
            {
                characterFormatting << "text-decoration:line-through";
            }
            /*
            else if (feature != CharStyle::INHERIT) // what is this good for?
                qDebug() << "else feature" << feature;
            */
            /*
             * The following other character formats are supported by Scribus but not retained
             * but not (yet) exported to epub:
             * OUTLINE: text-outline: 1px 1px #ccc;
             * SHADOWED: text-shadow: 2px 2px #ff0000;
             * ALLCAPS: text-transform:uppercase;
             * SMALLCAPS: font-variant:small-caps;
             * SHYPHEN: hyphenation is possible... how to use it?
             */
            // qDebug() << "tag name: " << element.tagName();

            characterStyleName = formatting.getCharacterStyleName();
            characterStyleName = getStylenameSanitized(characterStyleName);
            // qDebug() << "character style: " << characterStyleName;

            if (!characterFormatting.isEmpty() || (characterStyleName != ""))
            {
                element = xhtmlDocument.createElement("span");
                // qDebug() << "tag name: " << element.tagName();

                if (characterStyleName != "")
                    element.setAttribute("class", characterStyleName);

                if (!characterFormatting.isEmpty())
                    element.setAttribute("style", characterFormatting.join("; ") + ";");

                elementCurrent.appendChild(element);
                elementCurrent = element;
            }

            foreach (QVector<QString> content, run.content)
            {
                QVectorIterator<QString> list(content);
                while (list.hasNext()) {
                    QDomText elementText = xhtmlDocument.createTextNode(list.next());
                    elementCurrent.appendChild(elementText);
                    if (list.hasNext())
                        elementCurrent.appendChild(xhtmlDocument.createElement("br"));
                }
            }
            /*
            if (elementParagraph.text() != "")
                xhtmlBody.appendChild(elementParagraph);
            */

            elementCurrent = elementParagraph;
        }
        if (!elementParagraph.isNull())
            result.append(elementParagraph);
    }
    // qDebug() << "result" << result;
    return result;
}

QString ScribusAPIDocumentItemText::getStylenameSanitized(QString stylename)
{
    return stylename.remove(QRegExp("[^a-zA-Z\\d_-]"));
}



QDebug operator<<(QDebug dbg, ScribusAPIDocumentItemText &item)
{
    dbg.nospace() << "(name" << ")";
    // dbg.nospace() << "(name:" << item.getName() << ")";
    return dbg.space();
}

// not tested yet
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemTextRuns run)
{
    dbg.nospace() << "(position:" << run.position << ", lenght:" << run.length  << ", type:" << run.type  << ", content:" << run.content << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const QVector<ScribusAPIDocumentItemTextRuns> &runs)
{
    dbg.nospace() << "[" ;
    foreach (ScribusAPIDocumentItemTextRuns run, runs)
    {
        dbg.nospace() << run;
    }
    dbg.nospace() << "]" ;
    return dbg.space();
}
