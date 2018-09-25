// scribus/...
#include "selection.h"
#include "styles/styleset.h"
#include "styles/charstyle.h"

#include <vector>
#include <string>

#include "document.h"

#include "scribusdoc.h"
#include "scribusstructs.h" // TODO: for getPageRect() remove it, it's moved to ScPage

namespace ScribusAPI
{

nonstd::optional<Item> Document::getActiveItem()
{
	if (scribusDoc->m_Selection->count() != 1)
	{
		return {};
	}

	PageItem* current = scribusDoc->m_Selection->itemAt(0);
	auto item = Item(current);

	// item->setPageNumber(pageNumber); TODO: find out the page number

	return {item};
}

std::vector<std::string> Document::getCharacterStyleNames()
{
    std::vector<std::string> result{};

    const StyleSet<CharStyle>* charStyles = & scribusDoc->charStyles();
    int n = charStyles->count();
    for (int i = 0; i < n; ++i )
    {
        const auto charStyle = (*charStyles)[i];
        result.push_back(charStyle.name().toUtf8().constData());
    }

    return result;
}

std::vector<std::string> Document::getParagraphStyleNames()
{
    std::vector<std::string> result{};

    const StyleSet<ParagraphStyle>* paragraphStyles = & scribusDoc->paragraphStyles();
    int n = paragraphStyles->count();
    for (int i = 0; i < n; ++i )
    {
        const ParagraphStyle paragraphStyle = (*paragraphStyles)[i];
        result.push_back(paragraphStyle.name().toUtf8().constData());
    }

    return result;
}

void Document::readItems()
{
    if (!items.empty()) {
        return;
    }
    std::vector<int> layerNotPrintable;
    for (auto& layer: scribusDoc->Layers) {
        if (!layer.isPrintable) {
            layerNotPrintable.push_back(layer.ID);
        }
    }


    int n = this->getPageCount();
    // qDebug() << "readItems n: " << n;
    items.resize(n);
    for (auto scribusItem: scribusDoc->DocItems) {
		if (!scribusItem->printEnabled()) {
			continue;
        }

        if (std::find(layerNotPrintable.begin(), layerNotPrintable.end(), scribusItem->LayerID) != layerNotPrintable.end()) {
            continue;
        }

        const auto itemPages = getPagesWithItem(scribusItem);
    }

    /*
	foreach(PageItem* docItem, scribusDoc->DocItems)
    {
		if (!docItem->printEnabled())
			continue;
        if (layerNotPrintable.contains(docItem->LayerID))
            continue;

        auto itemPages = getPagesWithItem(docItem);
		// if the item is not on a page, ignore it
        if (itemPages.empty())
			continue;

        int pageNumber= itemPages.first()->pageNr();

        ScribusAPIDocumentItem* documentItem = new ScribusAPIDocumentItem(docItem);
        documentItem->setPageNumber(pageNumber);
        items[pageNumber].append(documentItem);
        // itemCounter++; eventually, for the progress bar... but we should probably count the pages
    }

    for (int i = 0; i < items.count(); i++)
        qSort(items[i].begin(), items[i].end(), ScribusAPIDocumentItem::isBeforeQSort);

    qDebug() << "items:" << items;
    */
}


/**
 * Returns a list of ScPage where the item appears. If the item is fully ion the scratch space,
 * an empty vector is returned.
 * TODO:
 * - open a ticket in the bug tracker and ask about setting the item's page(s) in the in-memory structure and in the sla file (updated manually (when needed: you have to sync if you need the precise information) or automatically (when changed)).
 * - This (or a similar) method should replace the (very) similar calculations in
 *   ScribusDoc::fixItemPageOwner, ScribusDoc::OnPage and PDFLibCore::PDF_ProcessItem
 *   It should go to PageItem (or ScPage) and it should be cached in memory + eventually in the .SLA
 *   --> PageItem::getPages()
 *   (According to jghali OwnPage should only be used make sense of the coordinates of an item,
 *   which are stored in relation to its own page)
 */
std::vector<ScPage *> Document::getPagesWithItem(PageItem* item)
{
    std::vector<ScPage *> result;

    // some woodoo adjustings
	if (item->isGroup())
		item->asGroupFrame()->adjustXYPosition();
	item->setRedrawBounding();

	double itemLineWidth = item->lineWidth();
    QRect pageRect;
    QRect itemRect = QRect(
        static_cast<int>(item->BoundingX - itemLineWidth / 2.0), // x
        static_cast<int>(item->BoundingY - itemLineWidth / 2.0), // y
        static_cast<int>(qMax(item->BoundingW + itemLineWidth, 1.0)), // w
        static_cast<int>(qMax(item->BoundingH + itemLineWidth, 1.0)) // h
    );

    bool fullyOnOwnPage = false;
    // First check if the element is fully on its OwnPage
    // OwnPage is an indicator of where the item could be, but it's not reliable.
    if (item->OwnPage > -1)
    {
        ScPage* page = scribusDoc->DocPages.at(item->OwnPage); // TODO: use the real page that we are handling
        if (getPageQRect(page).contains(itemRect)) {
            result.push_back(page);
            fullyOnOwnPage = true;
        }
    }

    // If the item is not fully on the OwnPage, check on all pages (in the range)
    if (!fullyOnOwnPage)
    {
        // TODO: if creating the QRect is expensive, we can create a list of pages' QRects
        // before cycling through the items
        bool allPages = pageRange.empty();
        int n = allPages ? getPageCount() : pageRange.size();
        for (int i = 0; i < n; ++i)
        {
            ScPage* page = scribusDoc->DocPages.at(allPages ? i : pageRange.at(i) - 1);
            if (getPageQRect(page).intersects(itemRect))
                result.push_back(page);
            // TODO: we can use rect.intersected() to get a rectangle and calculate the area of the page
            // that has the biggest intersection and use it as the "main page";
            // or we can use the first page where the intersection occurs (two different uses)
        }
        // TODO: if OwnPage == -1 and a page has been found, fix OwnPage in the item
    }

    return result;
}

/**
 * TODO:
 * Add it as ScPage::getRect(const ScPage* page)
 * Eventually, rename to signify that it does not return xOffset, yOffset, ... but it adds the bleeds
 */
QRect Document::getPageQRect(const ScPage* page)
{
    auto bleeds = getPageBleeds(page);
    return QRect(
        static_cast<int>(page->xOffset() - bleeds.left()), // x
        static_cast<int>(page->yOffset() - bleeds.top()), // y
        static_cast<int>(page->width() + bleeds.left() + bleeds.right()), // w
        static_cast<int>(page->height()+ bleeds.bottom() + bleeds.top()) // h
    );
}

/**
 * TODO:
 * add it as ScPage::getBleeds(const ScPage* page) and eventually remove/deprecate all the
 * ScribusDoc::getBleeds(...) methods.
 * Warning: in ScribusDoc there are also bleeds() methods that return the values without the facing
 * pages correction!
 */
MarginStruct Document::getPageBleeds(const ScPage* page)
{
    MarginStruct result;
    scribusDoc->getBleeds(page, result);
    return result;
}

}
