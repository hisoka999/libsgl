#pragma once

#include "engine/core/renderer.h"
#include "texture.h"

namespace graphics
{
    class ChildTexture
    {
    private:
        std::shared_ptr<graphics::Texture> texture;
        graphics::Rect rect;

    public:
        void render(const graphics::Rect &destRect, core::Renderer *renderer);
        ChildTexture(const std::shared_ptr<graphics::Texture> &texture,
                     const graphics::Rect &rect);
        ~ChildTexture();
    };

} // namespace graphics
