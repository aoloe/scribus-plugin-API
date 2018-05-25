#ifndef SCRIBUSAPIITEM_H
#define SCRIBUSAPIITEM_H

// scribus/...
#include "pageitem_textframe.h"

namespace ScribusAPI
{

class FrameText;

/**
 * Item is a proxy to the scribus' PageItem and providing access to the
 * items / frames.
 */
class Item
{
public:
    Item(PageItem* item) : scribusPageItem{item} {}
    // TODO: remove this: it's only here for debugging purposes
    Item() {}

    ~Item() {}

    /**
     * \brief Get the original Scribus item
     *
     * You get access to the Scribus item. You can access the
     * full item's features, without going through the API.
     */
    PageItem* getScribusItem() const { return scribusPageItem; }

    void setPageNumber(int pageNumber) { pageNumber = pageNumber; }

    int getX() const { return scribusPageItem->gXpos; }

    int getY() const { return scribusPageItem->gYpos; }

    bool isTextFrame() { return scribusPageItem->asTextFrame(); }

    FrameText getFrameText();


    bool isImageFrame() { return scribusPageItem->asImageFrame(); }

    std::string getName() { return scribusPageItem->itemName().toUtf8().constData(); }

private:
    int pageNumber;
    PageItem* scribusPageItem;

};

}

#endif // SCRIBUSAPIITEM_H 
