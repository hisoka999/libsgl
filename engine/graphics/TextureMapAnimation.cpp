#include "TextureMapAnimation.h"
#include "engine/utils/logger.h"
namespace graphics
{
    void TextureMapAnimation::renderFrame(AnimationFrame<std::string> &frame, const graphics::Rect &displayRect,
                                          core::Renderer *renderer)
    {

        graphics::Rect destRect;
        auto pos = currentPosition + utils::Vector2{displayRect.x, displayRect.y};

        destRect.x = pos.getX();
        destRect.y = pos.getY();
        destRect.width = displayRect.width;
        destRect.height = displayRect.height;

        m_textureMap->render(frame.data, destRect, renderer);
    }

    TextureMapAnimation::TextureMapAnimation(const utils::Vector2 &startPosition, const std::shared_ptr<TextureMap> &textureMap)
        : Animation<std::string>(startPosition), m_textureMap(textureMap)
    {
    }

    TextureMapAnimation::TextureMapAnimation()
        : Animation<std::string>()
    {
    }

    TextureMapAnimation::~TextureMapAnimation()
    {
    }

    void TextureMapAnimation::setTextureMap(const std::shared_ptr<TextureMap> &textureMap)
    {
        m_textureMap = textureMap;
    }
}