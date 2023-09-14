/*
 * TextAnimation.cpp
 *
 *  Created on: 09.09.2019
 *      Author: stefan
 */

#include <engine/graphics/TextAnimation.h>

namespace graphics
{

    TextAnimation::TextAnimation(utils::Vector2 startPosition, SDL_Color color, std::string text)
        : Animation<std::shared_ptr<graphics::Text>>(startPosition), color(color), text(text)
    {
        // TODO Auto-generated constructor stub
    }
    void TextAnimation::renderFrame(AnimationFrame<std::shared_ptr<graphics::Text>> &frame, const graphics::Rect &displayRect,
                                    core::Renderer *renderer)
    {
        auto pos = currentPosition + utils::Vector2{displayRect.x, displayRect.y};
        frame.data->render(renderer, text, color, pos.getX(), pos.getY());
    }
    TextAnimation::~TextAnimation()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace graphics */
