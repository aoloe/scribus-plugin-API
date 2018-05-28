#include "textFrame.h"

#include "scribusdoc.h"
#include "styles/paragraphstyle.h"

#include <QString>

namespace ScribusAPI
{

TextFrame::TextFrame(const Item &frame) : frame{frame} {}

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
void TextFrame::applyParagraphStyle(const std::string styleName)
{
	ParagraphStyle pstyle;
	pstyle.setParent(QString::fromStdString(styleName));
    // auto item = frame.getScribusItem();
    // item->itemText.applyStyle(0, pstyle);
    frame.getScribusItem()->doc()->itemSelection_ApplyParagraphStyle(pstyle);
}

}
