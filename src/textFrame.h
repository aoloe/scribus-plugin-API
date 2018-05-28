#ifndef SCRIBUSAPIFRAMETEXT_H
#define SCRIBUSAPIFRAMETEXT_H

#include "pageitem_textframe.h"

#include <string>

#include "item.h"

namespace ScribusAPI
{

class TextFrame
{
public:
    TextFrame(const Item& frame);
    ~TextFrame() {}

	void applyParagraphStyle(const std::string styleName);
	void applyCharacterStyle(const std::string styleName);
private:
    const Item& frame;
};

}

#endif // SCRIBUSAPIFRAMETEXT_H
