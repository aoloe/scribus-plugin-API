#ifndef SCRIBUSAPIFRAMETEXT_H
#define SCRIBUSAPIFRAMETEXT_H

#include "pageitem_textframe.h"

#include <string>
#include <vector>

#include "item.h"

namespace ScribusAPI
{

struct TextFrameRun
{
	int position;
	int length;
	char type; // p=paragraph, f=formatting
	std::vector<std::vector<std::string>> content;
	bool isParagraph() {return type == 'p';}
};

class TextFrame
{
public:
    TextFrame(const Item& frame);
    ~TextFrame() {}

	void applyParagraphStyle(const std::string& styleName);
	void applyCharacterStyle(const std::string& styleName);
	std::vector<TextFrameRun> getStoryRuns();
	std::vector<TextFrameRun> getRuns();
	std::vector<TextFrameRun> getRuns(int start, int end);
private:
    const Item& frame;
};

}

#endif // SCRIBUSAPIFRAMETEXT_H
