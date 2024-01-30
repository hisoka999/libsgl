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
        TextureMapAnimation();

        virtual ~TextureMapAnimation();

        void setTextureMap(const std::shared_ptr<TextureMap> &textureMap);
    };

    class TextureMapAnimator
    {
    private:
        std::map<std::string, TextureMapAnimation> m_animations;
        std::string m_currentAnimation;

    public:
        TextureMapAnimator(/* args */) {}
        ~TextureMapAnimator() {}
        void addAnimation(const std::string &name, TextureMapAnimation &animation)
        {
            m_animations[name] = animation;
        }

        void setCurrentAnimation(const std::string &name)
        {
            m_currentAnimation = name;
        }
        void render(core::Renderer *renderer, const graphics::Rect &displayRect)
        {
            if (m_currentAnimation.empty())
                return;
            m_animations[m_currentAnimation].render(renderer, displayRect);
        }

        void update()
        {
            if (m_currentAnimation.empty())
                return;
            m_animations[m_currentAnimation].update();
        } 

        TextureMapAnimation &operator[](const std::string &name)
        {
            return m_animations[name];
        }

        TextureMapAnimation &currentAnimation()
        {
            return m_animations[m_currentAnimation];
        }

        bool hasAnimation(const std::string name)
        {
            return m_animations.count(name) > 0;
        }
    };

} // namespace graphics
