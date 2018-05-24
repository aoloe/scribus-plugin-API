// scribus/...
// #include "selection.h"
// #include "styles/styleset.h"
// #include "styles/charstyle.h"
// #include <QString>

#include "document.h"

namespace ScribusAPI
{

nonstd::optional<Item> Document::getActiveItem()
{

    // TODO: temporary turn off the implementation
    auto item = Item{};
    /*
	if (scribusDoc->m_Selection->count() != 1)
	{
		return {};
	}

	PageItem* current = scribusDoc->m_Selection->itemAt(0);
	auto item = Item(current);
    */

	// item->setPageNumber(pageNumber); TODO: find out the page number

	return {item};
}

std::vector<std::string> Document::getCharacterStyleNames()
{
    std::vector<std::string> result{};

    // TODO: temporary turn off the implementation
    /*
    const StyleSet<CharStyle>* charStyles = & scribusDoc->charStyles();
    int n = charStyles->count();
    for (int i = 0; i < n; ++i )
    {
        const auto charStyle = (*charStyles)[i];
        result.push_back(charStyle.name().toStdString());
    }
    */
    return result;
}

std::vector<std::string> Document::getParagraphStyleNames()
{
    std::vector<std::string> result{};

    // TODO: temporary turn off the implementation
    /*
    const StyleSet<ParagraphStyle>* paragraphStyles = & scribusDoc->paragraphStyles();
    int n = paragraphStyles->count();
    for (int i = 0; i < n; ++i )
    {
        const ParagraphStyle paragraphStyle = (*paragraphStyles)[i];
        result.push_back(paragraphStyle.name().toStdString());
    }
    */
    return result;
}

}
