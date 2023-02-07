#include "TextureMapAnimation.h"
#include "engine/utils/logger.h"
namespace graphics
{
    void TextureMapAnimation::renderFrame(AnimationFrame<std::string> &frame, const utils::Vector2 &transform,
                                          core::Renderer *renderer)
    {

        graphics::Rect destRect;
        auto pos = currentPosition + transform;
        m_textureMap->getSourceRect(frame.data, &destRect);
        destRect.x = pos.getX();
        destRect.y = pos.getY();
        m_textureMap->render(frame.data, destRect, renderer);
    }

    TextureMapAnimation::TextureMapAnimation(const utils::Vector2 &startPosition, const std::shared_ptr<TextureMap> &textureMap)
        : Animation<std::string>(startPosition), m_textureMap(textureMap)
    {
    }

    TextureMapAnimation::~TextureMapAnimation()
    {
    }
}