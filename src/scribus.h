#ifndef SCRIBUSAPISCRIBUS_H
#define SCRIBUSAPISCRIBUS_H

#include <string>

#include "scribusdoc.h"

#include "document.h"

namespace ScribusAPI
{

class Scribus
{
    public:
        /**
         * \brief Returns the Qt main window. Useful for creating dialogs.
         *
         * ScribusMainWindow extends QMainWindow and is defined in scribus.h
         */
        static QMainWindow* getActiveWindow()
        {
            return ScCore->primaryMainWindow();
        }
        /**
         * \return An API document with a reference to the current document, if any.
         */
        static Document getActiveDocument()
        {
            // ScCore is a global variable created in main_*.cpp
            ScribusDoc* currDoc = ScCore->primaryMainWindow()->doc;
            if (currDoc == nullptr) {
                return {};
            }
            return Document{currDoc};
        }

        /**
         * \return An API document with a reference to the scribusDoc argument or to the current document, if any.
         */
        static Document getActiveDocument(ScribusDoc* scribusDoc)
        {
            if (scribusDoc == nullptr) {
                return getActiveDocument();
            } else {
                return Document{scribusDoc};
            }
        }

        /**
         * TODO: to be implemented
         */
        static Document getOpenDocument(int iDocument) { return {}; };

        /**
         * \brief return the number of open documents
         * TODO: to be implemented; see the scripter implementation
         */
        static int getDocumentsCount() { return {}; };

        /**
         * TODO: to be implemented
         */
        static Document createDocument(std::string filename) { return {}; };

        /**
         * TODO: to be implemented
         */
        static Document openDocument(std::string filename) { return {}; };
};

}

#endif // SCRIBUSAPISCRIBUS_H
