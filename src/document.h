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
         * Return the first selected item, if any.
         */
        nonstd::optional<Item> getActiveItem();

        /**
         * /brief Get the names of all character styles
         */
        std::vector<std::string> getCharacterStyleNames();

        /**
         * /brief Get the names of all paragraph styles
         */
        std::vector<std::string> getParagraphStyleNames();

    private:
        ScribusDoc* scribusDoc;
};

}

#endif // SCRIBUSAPIDOCUMENT_H
