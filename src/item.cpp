#include "item.h"

#include "textFrame.h"

namespace ScribusAPI
{

TextFrame Item::getTextFrame() { return TextFrame(*this); }

}
