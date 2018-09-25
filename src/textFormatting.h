#ifndef SCRIBUSAPITEXTFORMAT_H
#define SCRIBUSAPITEXTFORMAT_H

namespace ScribusAPI
{

enum TextFormattingFlag {
    formattingFlag_None          = 0,
    formattingFlag_Superscript   = 1 << 0, // 1
    formattingFlag_Subscript     = 1 << 1, // 2
    formattingFlag_Outline       = 1 << 2, // 4,
    formattingFlag_Underline     = 1 << 3, // 8
    formattingFlag_Strikethrough = 1 << 4,
    formattingFlag_AllCaps       = 1 << 5,
    formattingFlag_SmallCaps     = 1 << 6,
    formattingFlag_HyphenationPossible = 1 << 7, //Hyphenation possible here (Soft Hyphen)
    formattingFlag_Shadowed      = 1 << 8,
    formattingFlag_UnderlineWords= 1 << 9,
    formattingFlag_Reserved01    = 1 << 10, //free, not used in the moment
    formattingFlag_DropCap       = 1 << 11,
    formattingFlag_Bold          = 1 << 12, // not in scribus
    formattingFlag_Italic        = 1 << 13,
    formattingFlag_All           = (1 << 14) - 1,
};

/**
 * TextFormatting provides information and sets the formatting for the
 * current text item.
 */

class TextFormatting
{
    public:
        TextFormatting(Item& item) : item{item} {};
    private:
        Item& item;
};

} // ScribusAPI

#endif // SCRIBUSAPITEXTFORMAT_H
