#pragma once

#include <map>
#include <string>
#include "Animation.h"

namespace graphics
{

    template <typename T>
    class Animator
    {
    private:
        std::map<std::string, Animation<T>> animations;
        std::string currentAnimation;

    public:
        Animator(/* args */);
        void addAnimation(const std::string &name, Animation<T> &animation)
        {
            animations[name] = animation;
        }
        ~Animator();
        void setCurrentAnimation(const std::string &name)
        {
            currentAnimation = name;
        }
        void render(core::Renderer *renderer, const graphics::Rect &displayRect)
        {
            if (currentAnimation.empty())
                return;
            animations[currentAnimation].render(renderer, displayRect);
        }
    };

} // namespace graphics
