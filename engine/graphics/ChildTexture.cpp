#include "ChildTexture.h"

namespace graphics
{
    void ChildTexture::render(const graphics::Rect &destRect, core::Renderer *renderer)
    {
        texture->render(renderer, rect, destRect);
    }

    ChildTexture::ChildTexture(const std::shared_ptr<graphics::Texture> &texture,
                               const graphics::Rect &rect)
        : texture(texture), rect(rect)
    {
    }

    ChildTexture::~ChildTexture()
    {
    }
}