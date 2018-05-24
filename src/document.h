#ifndef SCRIBUSAPIDOCUMENT_H
#define SCRIBUSAPIDOCUMENT_H

#include "scribusdoc.h"

#include <functional> // if c++17+, optional.h needs hash to be defined
#include "third-party/optional/optional.h"
#include <vector>
#include <string>

#include "item.h"

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

        ~Document()
        {
        }

        bool isOpen() { return scribusDoc != nullptr; }

        /**
         * Return the first selected item, if any.
         */
        // nonstd::optional<Item> getActiveItem();
        // TODO: temporary avoid using the implementation in the cpp file
        nonstd::optional<Item> getActiveItem() { return Item{}; }

        /**
         * /brief Get the names of all character styles
         */
        // std::vector<std::string> getCharacterStyleNames();
        // TODO: temporary avoid using the implementation in the cpp file
        std::vector<std::string> getCharacterStyleNames() { return {}; }

        /**
         * /brief Get the names of all paragraph styles
         */
        // std::vector<std::string> getParagraphStyleNames();
        // TODO: temporary avoid using the implementation in the cpp file
        std::vector<std::string> getParagraphStyleNames() { return {}; }

    private:
        ScribusDoc* scribusDoc;
};

}

#endif // SCRIBUSAPIDOCUMENT_H
