#include "Hint.h"
namespace UI
{
    Hint::Hint() : position(0, 0), width(0), height(0)
    {
    }

    Hint::~Hint()
    {
    }

    void Hint::setPosition(const utils::Vector2 &position)
    {
        this->position = position;
    }

    const utils::Vector2 &Hint::getPosition()
    {
        return position;
    }

    size_t Hint::getWidth()
    {
        return width;
    }

    void Hint::setWidth(size_t width)
    {
        this->width = width;
    }

    size_t Hint::getHeight()
    {
        return height;
    }

    void Hint::setHeight(size_t height)
    {
        this->height = height;
    }

    void Hint::renderBackground(core::Renderer *renderer)
    {
        graphics::Rect rect = {position.getX(), position.getY(), float(width), float(height)};
        renderer->setDrawColor(255, 255, 255, 255);
        renderer->fillRect(rect);
        renderer->setDrawColor(200, 200, 200, 255);
        renderer->drawRect(rect);
    }

} // namespace UI
