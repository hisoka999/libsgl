#include "Hint.h"
#include "engine/graphics/TextureManager.h"

namespace UI
{
    Hint::Hint() : position(0, 0), width(0), height(0)
    {

        auto theme = graphics::TextureManager::Instance().getDefaultTheme();
        backgroundColor = theme->getStyleColor("hint", UI::StyleType::BackgroundColor);
        borderColor = theme->getStyleColor("hint", UI::StyleType::BorderColor);
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

    void Hint::setHintText(const std::string &hintText)
    {
        this->hintText = hintText;
    }

    void Hint::renderBackground(core::Renderer *renderer)
    {
        graphics::Rect rect = {position.getX(), position.getY(), float(width), float(height)};

        SDL_BlendMode oldBlendMode = renderer->getDrawBlendMode();
        renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer->setDrawColor(backgroundColor);
        renderer->fillRect(rect);
        renderer->setDrawColor(borderColor);
        renderer->drawRect(rect);
        renderer->setDrawBlendMode(oldBlendMode);
    }

    const std::string &Hint::getHintText()
    {
        return hintText;
    }

} // namespace UI
