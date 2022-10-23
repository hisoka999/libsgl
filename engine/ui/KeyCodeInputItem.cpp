#include "KeyCodeInputItem.h"
#include "engine/graphics/TextureManager.h"

namespace UI
{
    KeyCodeInputItem::KeyCodeInputItem(Object *parent, int pWidth, int pHeight)
        : UI::Object(parent, pWidth, pHeight), isSelected(false)
    {
        setObjectName("KeyCodeInputItem");
        SDL_StopTextInput();
        color = {255, 255, 255, 255};
        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }

        borderColor = getTheme()->getStyleColor(this, UI::StyleType::BorderColor);
        hoverColor = getTheme()->getStyleColor(this, UI::StyleType::HoverColor);

        backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
    }

    KeyCodeInputItem::~KeyCodeInputItem()
    {
    }

    SDL_Keycode KeyCodeInputItem::getValue()
    {
        return m_keyCode;
    }

    void KeyCodeInputItem::setValue(const SDL_Keycode pValue)
    {
        m_keyCode = pValue;
        fireFuncionCall("keyCodeChanged", m_keyCode);
    }

    void KeyCodeInputItem::render(core::Renderer *pRender)
    {
        graphics::Rect rect = displayRect();

        pRender->setDrawColor(backgroundColor);

        pRender->fillRect(rect);

        if (isSelected)
        {
            pRender->setDrawColor(hoverColor);
        }
        else
        {
            pRender->setDrawColor(borderColor);
        }
        pRender->drawRect(rect);
        std::string text = SDL_GetKeyName(m_keyCode);
        int textW, textH = 0;
        if (text.size() != 0)
        {

            getFont()->size(text, &textW, &textH);
            getFont()->render(pRender, text, color, rect.x + 5, rect.y + (rect.height / 2.0f) - (textH / 2.0f));
        }
    }

    bool KeyCodeInputItem::handleEvents(core::Input *pInput)
    {
        bool eventHandled = false;
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            if (eventRect().intersects(pInput->getMousePostion()))
            {
                isSelected = true;
                eventHandled = true;
            }
            else
            {
                if (isSelected)
                {
                    isSelected = false;
                }
            }
        }
        else if (isSelected)
        {
            SDL_Keycode keyCode = 0;
            bool keyDown = false;
            pInput->keyDown(&keyDown, &keyCode);
            if (keyDown)
            {
                setValue(keyCode);
                eventHandled = true;
            }
        }
        return eventHandled;
    }

    void KeyCodeInputItem::setColor(int r, int g, int b)
    {
        color.r = r;
        color.g = g;
        color.b = b;
    }
}