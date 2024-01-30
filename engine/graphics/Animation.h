/*
 * Animation.h
 *
 *  Created on: 15.06.2019
 *      Author: stefan
 */

#ifndef GRAPHICS_ANIMATION_H_
#define GRAPHICS_ANIMATION_H_

#include <SDL2/SDL.h>
#include <engine/core/renderer.h>
#include <engine/utils/vector2.h>
#include <engine/utils/logger.h>
#include <vector>

namespace graphics
{

    template <typename T>
    struct AnimationFrame
    {
        utils::Vector2 position;
        size_t time;
        T data;

        AnimationFrame(utils::Vector2 &pos, size_t time, T data)
        {
            position = pos;
            this->time = time;
            this->data = data;
        }
    };

    template <typename T>
    class Animation
    {
    public:
        Animation(utils::Vector2 startPosition);
        Animation();
        virtual ~Animation();

        bool isPlaying();
        void play();
        void stop();
        void pause();

        void update();
        void render(core::Renderer *renderer, const graphics::Rect &displayRect);
        void addFrame(AnimationFrame<T> frame);
        template <typename X>
        void createFrame(utils::Vector2 &pos, size_t time, X data)
        {
            AnimationFrame<X> frame(pos, time, data);
            frames.push_back(frame);
            currentFrame = 0;
        }
        void setRepeating(int repeat);
        void setStartPosition(const utils::Vector2 &startPosition);

    protected:
        virtual void renderFrame(AnimationFrame<T> &frame, const graphics::Rect &displayRect,
                                 core::Renderer *renderer) = 0;
        utils::Vector2 currentPosition;

    private:
        std::vector<AnimationFrame<T>> frames;
        bool playing;
        int currentFrame;
        size_t startTime = 0;

        utils::Vector2 startPosition;
        int repeat = 0;
    };

} /* namespace graphics */

template <typename T>
inline graphics::Animation<T>::Animation(utils::Vector2 startPosition)
    : playing(false), currentFrame(0), startPosition(startPosition)
{
}

template <typename T>
inline graphics::Animation<T>::Animation()
    : playing(false), currentFrame(0), startPosition(0.f, 0.f)
{
}

template <typename T>
inline graphics::Animation<T>::~Animation()
{
}

template <typename T>
inline bool graphics::Animation<T>::isPlaying()
{
    return playing;
}

template <typename T>
inline void graphics::Animation<T>::setRepeating(int repeat)
{
    this->repeat = repeat;
}

template <typename T>
inline void graphics::Animation<T>::play()
{
    currentFrame = 0;
    playing = true;
    startTime = SDL_GetTicks();
    currentPosition = startPosition;
}

template <typename T>
inline void graphics::Animation<T>::stop()
{
    currentFrame = 0;
    playing = false;
}

template <typename T>
inline void graphics::Animation<T>::pause()
{
    playing = false;
}

template <typename T>
inline void graphics::Animation<T>::update()
{
    if (!isPlaying())
        return;
    auto &nextFrame = frames.at(currentFrame + 1);
    size_t time = SDL_GetTicks() - startTime;
    currentPosition = utils::lerp(frames[currentFrame].position,
                                  nextFrame.position,
                                  ((float)nextFrame.time - (float)time) / (float)nextFrame.time);
    if (time > frames.at(currentFrame).time)
    {
        currentFrame++;
        startTime = SDL_GetTicks();
        if (currentFrame >= int(frames.size() - 1))
        {
            if (repeat > 0)
                repeat--;
            if (repeat == 0)
                stop();

            currentFrame = 0;
        }
    }
}

template <typename T>
inline void graphics::Animation<T>::render(core::Renderer *renderer, const graphics::Rect &displayRect)
{
    if (currentFrame >= 0)
        renderFrame(frames[currentFrame], displayRect, renderer);
    else
        SGL_LOG_ERROR("no frame to render");
}

template <typename T>
inline void graphics::Animation<T>::addFrame(AnimationFrame<T> frame)
{
    frames.push_back(frame);
    currentFrame = 0;
}
template <typename T>
inline void graphics::Animation<T>::setStartPosition(const utils::Vector2 &pos)
{
    startPosition = pos;
}
#endif /* GRAPHICS_ANIMATION_H_ */
