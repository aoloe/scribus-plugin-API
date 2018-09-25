#include "textFrame.h"

#include "scribusdoc.h"
#include "styles/paragraphstyle.h"
#include "styles/charstyle.h"

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
void TextFrame::applyParagraphStyle(const std::string& styleName)
{
	ParagraphStyle style;
	style.setParent(QString::fromStdString(styleName));
    frame.getScribusItem()->doc()->itemSelection_ApplyParagraphStyle(style);
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
void TextFrame::applyCharacterStyle(const std::string& styleName)
{
	CharStyle style;
	style.setParent(QString::fromStdString(styleName));
    frame.getScribusItem()->doc()->itemSelection_ApplyCharStyle(style);
}

/**
 * @bried Get the runs in the full story attached to the text frame
 *
 * @see getRuns(int start, int end) for more details.
 *
 */
std::vector<TextFrameRun> TextFrame::getStoryRuns()
{
	return getRuns(0, frame.getScribusItem()->itemText.length());
}

/**
 * @brief Get the runs in the text frame.
 *
 * @see getRuns(int start, int end) for more details.
 */
std::vector<TextFrameRun> TextFrame::getRuns()
{
	return getRuns(frame.getScribusItem()->firstInFrame(), frame.getScribusItem()->lastInFrame());
}

/**
 * @brief Parse the text in a specific range in the sotry and return its runs.
 *
 * Runs are strings with a common formatting. The paragraph mark ends the run.
 *
 * This method is based on Pierre's work for the mitical OIF branch
 * Inspirations also comes from Scribus150Format::writeITEXTs
 * @todo:
 * - use the text/storytext methods as soon as they are implemented
 */
std::vector<TextFrameRun> TextFrame::getRuns(int start, int end)
{
	std::vector<TextFrameRun> runs;
	return runs;
}


}
