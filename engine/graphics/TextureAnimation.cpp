/*
 * TextureAnimation.cpp
 *
 *  Created on: 15.06.2019
 *      Author: stefan
 */

#include <engine/graphics/TextureAnimation.h>

namespace graphics
{

    TextureAnimation::TextureAnimation(utils::Vector2 startPosition) : Animation<std::shared_ptr<graphics::Texture>>(startPosition)
    {
        // TODO Auto-generated constructor stub
    }

    TextureAnimation::~TextureAnimation()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace graphics */

void graphics::TextureAnimation::renderFrame(
    AnimationFrame<std::shared_ptr<graphics::Texture>> &frame, const utils::Vector2 &transform, core::Renderer *renderer)
{
    auto pos = currentPosition + transform;
    frame.data->render(renderer, pos.getX(),
                       pos.getY());
}
