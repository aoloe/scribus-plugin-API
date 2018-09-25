#ifndef SCRIBUSAPIDOCUMENT_H
#define SCRIBUSAPIDOCUMENT_H

#include "scribusdoc.h"

#include <functional> // if c++17+, optional.h needs hash to be defined
#include "third-party/optional/optional.h"
#include <vector>
#include <string>

#include "item.h"

class ScPage; // TODO: for getPageRect() remove it, it's moved to ScPage
class MarginStruct; // TODO: for getPageRect() remove it, it's moved to ScPage


namespace ScribusAPI
{

class Document
{
    public:

        /**
         * \brief Initialize an API document without a Scribus. Don't use this for normal cases.
         * TODO: should this be private?
         */
        Document() {}

        /**
         * \brief Initialize an API document with a Scribus "core" document.
         * \param doc A pointer to a ScribusDoc structure passed to the plugin as an argument.
         */
        Document(ScribusDoc* scribusDoc) :
            scribusDoc{scribusDoc}
        {
        }

        /**
         * \brief Open the Scribus file if it exists or create it.
         * \param filename Absolute path to the file
         * TODO: in this case we have to delete the scribusDoc in the destructor. or let scribus add it to the list of docs and manage its closing.
         */
        /*
        Document(std::string filename)
        {
        }
        */

        ~Document()
        {
        }

        bool isOpen() { return scribusDoc != nullptr; }

        /**
         * /brief Get the names of all character styles
         */
        std::vector<std::string> getCharacterStyleNames();

        /**
         * /brief Get the names of all paragraph styles
         */
        std::vector<std::string> getParagraphStyleNames();

        /**
         * \brief The number of pages in the document
         */
        int getPageCount() { return this->scribusDoc->DocPages.count(); }
        /**
         * \brief Get the page number as defined in the current section.
         * \param i The index of the page in the document.
         */
        std::string getPageNumber(int i) { return scribusDoc->getSectionPageNumberForPageIndex(i).toUtf8().constData(); }

        /**
         * Return the first selected item, if any.
         */
        nonstd::optional<Item> getActiveItem();

        std::vector<Item> getPageItems(int page) { readItems(); return items.at(page);}

    private:
        ScribusDoc* scribusDoc;
        std::vector<std::vector<Item>> items{};
        void readItems();
        /**
         * \brief List of pages where an item shows up
         */
        std::vector<ScPage *> getPagesWithItem(PageItem* item);
        QRect getPageQRect(const ScPage* page);
        MarginStruct getPageBleeds(const ScPage* page);
        // TODO: in the previous version, typically from the options... how to manage it now?
        std::vector<int> pageRange;
};

}

#endif // SCRIBUSAPIDOCUMENT_H
