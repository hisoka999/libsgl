#include "StringHint.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/color.h>

namespace UI
{
    StringHint::StringHint() : Hint()
    {
        font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
    }

    StringHint::~StringHint()
    {
    }

    void StringHint::render(core::Renderer *renderer)
    {
        int w, h;
        font->size(getHintText(), &w, &h);

        int lineCount = 1;
        for (char chr : getHintText())
        {
            if (chr == '\n')
                lineCount++;
        }

        setWidth(w);
        setHeight(h * lineCount);

        renderBackground(renderer);

        font->renderWrapped(renderer, getHintText(), utils::color::BLUE, getPosition().getX(), getPosition().getY(), 150);
    }

} // namespace UI
