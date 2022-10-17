#include "engine/ui/Button.h"
#include <engine/graphics/TextureManager.h>
#include <functional>

namespace UI
{

    Button::Button(Object *parent)
        : Object(parent), static_width(0), hover(false), enabled(true), borderless(false)
    {
        setObjectName("button");

        setWidth(static_width);
        if (parent == nullptr || getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }

        color = getTheme()->getStyleColor(this, UI::StyleType::Color);

        disabledColor = getTheme()->getStyleColor(this, UI::StyleType::DisabledColor);

        hoverColor = getTheme()->getStyleColor(this, UI::StyleType::HoverColor);

        auto textureName = getTheme()->getStyleText(this, UI::StyleType::BackgroundImage);
        texture = nullptr;

        if (!textureName.empty())
            texture = graphics::TextureManager::Instance().loadTexture(textureName);
    }

    Button::~Button()
    {
    }

    void Button::setStaticWidth(const int pWidth)
    {
        static_width = pWidth;
        if (static_width > getWidth())
        {
            setWidth(static_width);
        }
    }

    void Button::setFont(const std::string &fontname, unsigned int font_size)
    {
        // TODO error handling
        auto text = graphics::TextureManager::Instance().loadFont(fontname, font_size);
        Object::setFont(text.get());
    }
    void Button::setColor(SDL_Color color)
    {
        this->color = color;
    }

    void Button::setLabel(const std::string &label)
    {
        this->label = label;

        int w = 0;
        int h = 0;
        if (getFont()->size(label, &w, &h) != -1)
        {
            if (w > static_width)
            {
                setWidth(w);
            }
            else
            {
                setWidth(static_width);
            }

            setHeight(35);
        }
    }
    graphics::Rect Button::displayRect()
    {
        graphics::Rect r;

        if (getParent() != nullptr)
        {
            r = getParent()->displayRect();

            r.x += float(getX());
            r.y += float(getY());
        }
        else
        {
            r.x = float(getX());
            r.y = float(getY());
        }
        r.width = float(getWidth() + 25);
        r.height = float(getHeight());
        return r;
    }

    graphics::Rect Button::eventRect()
    {
        graphics::Rect r;

        if (getParent() != nullptr)
        {
            r = getParent()->eventRect();

            r.x += float(getX());
            r.y += float(getY());
        }
        else
        {
            r.x = float(getX());
            r.y = float(getY());
        }
        r.width = float(getWidth() + 25);
        r.height = float(getHeight());
        return r;
    }

    bool Button::getBorderless() const
    {
        return borderless;
    }

    void Button::setBorderless(bool value)
    {
        borderless = value;
    }
    bool Button::handleEvents(core::Input *pInput)
    {
        if (!enabled)
        {
            hover = false;
            return false;
        }
        bool eventHandled = false;
        graphics::Rect r = eventRect();

        if (r.intersects(pInput->getMousePostion()))
        {
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                this->fireFuncionCall(buttonClickCallback());
                eventHandled = true;
            }
            hover = true;
        }
        else
        {
            hover = false;
        }
        Object::handleEvents(pInput);
        return eventHandled;
    }

    void Button::renderBackground(core::Renderer *pRenderer)
    {
        int tx, ty;
        if (getParent() != nullptr)
        {
            graphics::Rect r = getParent()->displayRect();

            r.x += getX();
            r.y += getY();
            tx = int(r.x);
            ty = int(r.y);
        }
        else
        {
            tx = getX();
            ty = getY();
        }
        if (!borderless)
        {

            // draw background rect
            graphics::Rect backgroundRect;
            backgroundRect.x = float(tx);
            backgroundRect.y = float(ty);
            backgroundRect.width = float(getWidth() + 25);
            backgroundRect.height = float(getHeight());

            pRenderer->setDrawColor(getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor));
            pRenderer->fillRect(backgroundRect);

            // left top corner
            if (texture != nullptr)
            {
                texture->render(pRenderer, tx, ty, 9, 9, 0, 0);
                // left bottom corner
                texture->render(pRenderer, tx, ty + getHeight() - 9, 9, 9, 0, 20);

                texture->render(pRenderer, tx + getWidth() + 25 - 9, ty, 9, 9, 174, 0);
                texture->render(pRenderer, tx + getWidth() + 25 - 9, ty + getHeight() - 9, 9, 9, 174, 20);
            }
            else
            {
                pRenderer->setDrawColor(getTheme()->getStyleColor(this, UI::StyleType::BorderColor));
                pRenderer->drawRect(backgroundRect);
            }
        }
        Object::render(pRenderer);
    }

    SDL_Color Button::getDisabledColor() const
    {
        return disabledColor;
    }

    void Button::setDisabledColor(const SDL_Color &value)
    {
        disabledColor = value;
    }

    bool Button::isToggleAllowed()
    {
        return toggleAllowed;
    }

    bool Button::isToggled()
    {
        return toggled && toggleAllowed;
    }

    void Button::setToggleAllowed(bool toggleAllowed)
    {
        this->toggleAllowed = toggleAllowed;
    }

    void Button::toggle()
    {
        toggled = !toggled;
    }

    void Button::setToggled(bool value)
    {
        toggled = value;
    }

    SDL_Color Button::getHoverColor() const
    {
        return hoverColor;
    }

    void Button::setHoverColor(const SDL_Color &value)
    {
        hoverColor = value;
    }

    bool Button::isHovered() const
    {
        return hover;
    }

    void Button::render(core::Renderer *pRender)
    {

        int tx;
        SDL_Color displayColor = color;

        if (!enabled)
        {
            displayColor = disabledColor;
        }
        else if (hover || isToggled())
        {
            displayColor = hoverColor;
        }

        int ty;
        graphics::Rect r = displayRect();

        tx = int(r.x);
        ty = int(r.y);
        renderBackground(pRender);

        getFont()->render(pRender, label, displayColor, tx + 10, ty + 10);
    }

    void Button::enable()
    {
        enabled = true;
    }
    void Button::disable()
    {
        enabled = false;
    }
    bool Button::isEnabled()
    {
        return enabled;
    }

} // namespace UI
