#pragma once

#include "Animation.h"
#include "texturemap.h"

namespace graphics
{
    class TextureMapAnimation : public Animation<std::string>
    {
    private:
        std::shared_ptr<TextureMap> m_textureMap;

    protected:
        void renderFrame(AnimationFrame<std::string> &frame, const graphics::Rect &displayRect,
                         core::Renderer *renderer) override;

    public:
        TextureMapAnimation(const utils::Vector2 &startPosition, const std::shared_ptr<TextureMap> &textureMap);
        virtual ~TextureMapAnimation();
    };

} // namespace graphics
