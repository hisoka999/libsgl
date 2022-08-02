#include "StringHint.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/color.h>

namespace UI
{
    StringHint::StringHint() : Hint()
    {
        font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 14);
    }

    StringHint::StringHint(const std::string &hintText)
        : Hint()
    {
        font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 14);
        setHintText(hintText);
    }

    StringHint::~StringHint()
    {
    }

    void StringHint::render(core::Renderer *renderer)
    {
        int w, h = 0;

        int lineCount = 1;
        int lastLineBreak = 0;
        int pos = 0;
        int maxWidth = 0;
        for (char chr : getHintText())
        {

            if (chr == '\n')
            {
                lineCount++;
                std::string line = getHintText().substr(lastLineBreak, pos - lastLineBreak);
                font->size(line, &w, &h);
                maxWidth = std::max(w, maxWidth);
                lastLineBreak = pos;
            }
            pos++;
        }
        if (lastLineBreak == 0)
            font->size(getHintText(), &w, &h);

        maxWidth = std::max(w, maxWidth);
        setWidth(maxWidth);
        setHeight(h * lineCount);

        renderBackground(renderer);

        font->renderWrapped(renderer, getHintText(), utils::color::WHITE, getPosition().getX(), getPosition().getY(), 200);
    }

} // namespace UI
