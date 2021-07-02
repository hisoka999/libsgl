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
        font->size(hintText, &w, &h);
        setWidth(w);
        setHeight(h);

        renderBackground(renderer);

        font->render(renderer, hintText, utils::color::WHITE, getPosition().getX(), getPosition().getY());
    }

} // namespace UI
