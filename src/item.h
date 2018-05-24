#ifndef SCRIBUSAPITITEM_H
#define SCRIBUSAPITITEM_H

#include "pageitem_textframe.h"

namespace ScribusAPI
{

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

    PageItem* getScribusItem() { return scribusPageItem; }

    void setPageNumber(int pageNumber) { pageNumber = pageNumber; }

    int getX() const { return scribusPageItem->gXpos; }

    int getY() const { return scribusPageItem->gYpos; }

    bool isTextFrame() { return scribusPageItem->asTextFrame(); }

    bool isImageFrame() { return scribusPageItem->asImageFrame(); }

    std::string getName() { return scribusPageItem->itemName().toUtf8().constData(); }

private:
    int pageNumber;
    PageItem* scribusPageItem;

};

}

#endif //SCRIBUSAPIITEM_H 
