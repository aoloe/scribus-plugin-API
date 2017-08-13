#include "scribusAPIDocumentItemText.h"

#include <QDebug>
#include <QVector>
#include <QList>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include "scribusAPIDocumentItem.h"
#include "scribusAPIDocumentItemTextFormatting.h"

#include "styles/paragraphstyle.h"

ScribusAPIDocumentItemText::ScribusAPIDocumentItemText(ScribusAPIDocumentItem* documentItem) : documentItem{documentItem}
{
}

ScribusAPIDocumentItemText::~ScribusAPIDocumentItemText()
{
}


/**
 * @brief Get the runs for the full story of the text frame.
 *
 * When looping through all the items in a document, you will probably want to check
 * if the current item is the beginning of a chain before calling getStoryRuns().
 *
 * @see getRuns(int start, int end) for more details.
 */
QVector<ScribusAPIDocumentItemTextRuns> ScribusAPIDocumentItemText::getStoryRuns()
{
	return getRuns(0, documentItem->getItem()->itemText.length());
}

/**
 * @brief Get the runs in the text frame.
 *
 * @see getRuns(int start, int end) for more details.
 */
QVector<ScribusAPIDocumentItemTextRuns> ScribusAPIDocumentItemText::getRuns()
{
	return getRuns(documentItem->getItem()->firstInFrame(), documentItem->getItem()->lastInFrame());
}

/**
 * @brief Parse the text and define the runs list.
 * @brief Get the runs for a specific range in the story.
 *
 * Runs are strings with a common formatting. The paragraph mark ends the run.
 *
 * this method is based on pierre's work for the mitical OIF branch
 * ... also inspired by Scribus150Format::writeITEXTs
 * @todo:
 * - use the text/storytext methods as soon as they are implemented
 */
QVector<ScribusAPIDocumentItemTextRuns> ScribusAPIDocumentItemText::getRuns(int start, int end)
{
	QVector<ScribusAPIDocumentItemTextRuns> runs; // the indexes where the runs start
	struct ScribusAPIDocumentItemTextRuns runsItem;
	runs.clear();

	CharStyle lastStyle;
	int lastPos = start;
	runsItem.position = start;
	runsItem.type = 'p';
	runsItem.content.clear();

	StoryText itemText = documentItem->getItem()->itemText;

	QVector<QString> lines;
	QString line = "";

	if (end == 0) {
		runsItem.length = 0;
		runs.append(runsItem);
	} else {
		lines.clear();

		for(int i = start; i < end; ++i)
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
		runsItem.length = end - runsItem.position;
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
 *   - eventually allow to get the dom of the current frame only (not the full story)
 */
QList<QDomElement> ScribusAPIDocumentItemText::getDom(QDomDocument xhtmlDocument)
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
    ScribusAPIDocumentItemTextFormatting formatting(documentItem);
    // qDebug() << "isFirstInChain" << isFirstInChain(); 
    // qDebug() << "isEmpty" << isEmpty(); 

    if (isFirstInChain() && (!isEmpty()))
    {
        foreach (ScribusAPIDocumentItemTextRuns run, getStoryRuns())
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

/**
 * @brief Get the number of characters in the frame
 */
int ScribusAPIDocumentItemText::getLength()
{
    PageItem* item = documentItem->getItem();
    return item->lastInFrame() - item->firstInFrame();;
}

int ScribusAPIDocumentItemText::getStoryLength()
{
    return documentItem->getItem()->itemText.length();

}

/**
 * @brief  Insert text at the current cursor position.
 *
 * Insert the text before the cursor position.
 *
 * Use append() to insert text after the last character.
 */
void ScribusAPIDocumentItemText::insert(const QString text, const QString paragraphStyle, const QString characterStyle)
{
}

/**
 * @brief Append text at the end of the chain of text frames.
 *
 * The cursor is moved to the end of the text in the chain.
 * @todo: implement it.
 */
void ScribusAPIDocumentItemText::append(const QString text, const QString paragraphStyle, const QString characterStyle)
{
}

/**
 * @brief Append a paragraph at the end of the chain of text frames.
 *
 * If the story is not empty, add a new line before inserting the text.
 * The cursor does not move.
 */
void ScribusAPIDocumentItemText::appendParagraph(const QString text, const QString paragraphStyle)
{
	PageItem* item = documentItem->getItem();
	int pos = item->itemText.length();
	if (!isEmpty())
	{
		item->itemText.insertChars(pos, SpecialChars::PARSEP);
		pos++;
	}
	if (paragraphStyle != "") {
		applyParagraphStyle(paragraphStyle);
	}
	item->itemText.insertChars(pos, text);
}

/**
 * @brief  Apply the paragraph style at the current cursor position.
 *
 * @todo:
 * - apply the style depending on the "type" of the current selection:
 *   - to the whole frame if a frame is selected
 *   - to the whole paragraphs in the selection if there is a selection
 *   - to the whole current paragraph if there is no selection but the cursor is in edit mode.
 *   - have a look at `void ScribusDoc::itemSelection_SetNamedCharStyle(const QString& name, Selection* customSelection)`
 *   - a different approach might use the story-applyStyle(pos, style)  used in gtaction.cpp
 *   - it probably only works if we are already in edit mode and the cursor has a position
 *
 *
 * a good example for applying the paragraph style seems to be
 * void ScribusMainWindow::setNewCharStyle(const QString& name)
 * in scribus.cpp, which is called from the properties palette.
 *
 */
void ScribusAPIDocumentItemText::applyParagraphStyle(const QString styleName)
{
    PageItem* item = documentItem->getItem();
	ParagraphStyle pstyle;
	pstyle.setParent(styleName);
    // item->itemText.applyStyle(0, pstyle);
    documentItem->getItem()->doc()->itemSelection_ApplyParagraphStyle(pstyle);
}

/**
 * @brief  Apply the paragraph style at a specific position.
 */
void ScribusAPIDocumentItemText::applyParagraphStyle(int position, const QString styleName)
{

    qDebug() << "apply p style" << styleName;

    /*

	ParagraphStyle pstyle;
	pstyle.setParent(styleName);
	// tocFrame->itemText.applyStyle(pos, pstyle);
    documentItem->getItem()->applyStyle(0, pstyle);
    */
    // cf: void ScribusDoc::itemSelection_SetNamedParagraphStyle(const QString& name, Selection* customSelection) for grocking how to "manually" apply the style; customSelection is by default 0
    // doc->itemSelection_SetNamedParagraphStyle(name);
    // TODO: how to access the document from item?
    // TODO: get selection and pass it as the second parameter for itemSelection_ApplyParagraphStyle()
    // TODO: to the same for the character styles
    // Selection tempSelection(this, false);
    // tempSelection.addItem(item, true);
    PageItem* item = documentItem->getItem();
	ParagraphStyle pstyle;
	pstyle.setParent(styleName);
    documentItem->getItem()->doc()->itemSelection_ApplyParagraphStyle(pstyle);
    item->itemText.applyStyle(position, pstyle);
}

/**
 * @brief  Apply the character style at the current cursor position.
 *
 * @todo:
 * - apply the style depending on the "type" of the current selection:
 *   - to the whole frame if a frame is selected
 *   - to the whole paragraphs in the selection if there is a selection
 *   - to the whole current paragraph if there is no selection but the cursor is in edit mode.
 *   - have a look at `void ScribusDoc::itemSelection_SetNamedCharStyle(const QString& name, Selection* customSelection)`
 */
void ScribusAPIDocumentItemText::applyCharacterStyle(const QString styleName)
{

    qDebug() << "apply c style" << styleName;

    /*
    PageItem* item = documentItem->getItem();
	CharStyle cstyle;
	newStyle.setParent(name);
	itemSelection_ApplyCharStyle(newStyle);
    documentItem->getItem()->doc()->itemSelection_ApplyCharacterStyle(cstyle);
    */
    documentItem->getItem()->doc()->itemSelection_SetNamedCharStyle(styleName);
    // PageItem *currItem = documentItem->getItem()->doc()->m_Selection->itemAt(0);
    // ScribusMainWindow::setTBvals(currItem);

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
