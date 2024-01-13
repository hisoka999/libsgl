/*
 * TextureAnimation.h
 *
 *  Created on: 15.06.2019
 *      Author: stefan
 */

#ifndef GRAPHICS_TEXTUREANIMATION_H_
#define GRAPHICS_TEXTUREANIMATION_H_

#include <engine/graphics/Animation.h>
#include <engine/graphics/texture.h>

namespace graphics
{

    class TextureAnimation : public Animation<std::shared_ptr<graphics::Texture>>
    {
    public:
        TextureAnimation(utils::Vector2 startPosition);
        virtual ~TextureAnimation();

    protected:
        void renderFrame(AnimationFrame<std::shared_ptr<graphics::Texture>> &frame, const graphics::Rect &displayRect,
                         core::Renderer *renderer) override;
    };

} /* namespace graphics */

#endif /* GRAPHICS_TEXTUREANIMATION_H_ */
