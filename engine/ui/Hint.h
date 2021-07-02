#ifndef __HINT_H__
#define __HINT_H__

#include <engine/core/renderer.h>
#include <engine/utils/vector2.h>

namespace UI
{
    class Hint
    {
    public:
        Hint();
        ~Hint();

        virtual void render(core::Renderer *renderer) = 0;
        void setPosition(const utils::Vector2 &position);
        const utils::Vector2 &getPosition();
        size_t getWidth();
        void setWidth(size_t width);
        size_t getHeight();
        void setHeight(size_t height);
        void setHintText(const std::string &hintText);

    protected:
        void renderBackground(core::Renderer *renderer);
        const std::string &getHintText();

    private:
        std::string hintText;
        utils::Vector2 position;
        size_t width;
        size_t height;
    };
} // namespace UI

#endif // __HINT_H__