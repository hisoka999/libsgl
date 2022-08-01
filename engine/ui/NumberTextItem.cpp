#include "NumberTextItem.h"
#include "engine/utils/string.h"
#include "engine/utils/localisation.h"
#include <engine/graphics/TextureManager.h>
namespace UI
{
    NumberTextItem::NumberTextItem(Object *parent, int pWidth, int pHeight)
        : UI::Object(parent, pWidth, pHeight), isSelected(false)
    {
        setObjectName("NumberTextItem");
        // TODO Auto-generated constructor stub
        SDL_StopTextInput();
        color = {255, 255, 255, 255};
        cursorPosition = 0;
        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }

        borderColor = getTheme()->getStyleColor(this, UI::StyleType::BorderColor);
        backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
    }

    NumberTextItem::~NumberTextItem()
    {
    }

    float NumberTextItem::getValue()
    {
        return value;
    }
    void NumberTextItem::setValue(const float pValue)
    {
        this->value = pValue;
        text = utils::string_format("%.2f", value);
        cursorPosition = text.size();
        fireFuncionCall("valueChanged", value);
    }

    void NumberTextItem::render(core::Renderer *pRender)
    {
        graphics::Rect rect = displayRect();

        pRender->setDrawColor(backgroundColor);

        pRender->fillRect(rect);
        pRender->setDrawColor(borderColor);
        pRender->drawRect(rect);

        int textW, textH = 0;
        if (text.size() != 0)
        {

            getFont()->size(text, &textW, &textH);
            getFont()->render(pRender, text, color, rect.x + 5, rect.y + (rect.height / 2.0f) - (textH / 2.0f));
        }
        std::string textToCursor = text.substr(0, cursorPosition);
        getFont()->size(textToCursor, &textW, &textH);
        utils::Vector2 lineStart(textW + rect.x + 2, rect.y);
        utils::Vector2 lineEnd(textW + rect.x + 2, rect.y + textH);
        pRender->drawLine(lineStart, lineEnd);
    }
    bool NumberTextItem::handleEvents(core::Input *pInput)
    {
        bool eventHandled = false;
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            if (eventRect().intersects(pInput->getMousePostion()))
            {
                SDL_Rect rect = displayRect().sdlRect();
                SDL_SetTextInputRect(&rect);
                SDL_StartTextInput();
                isSelected = true;
                eventHandled = true;
            }
            else
            {
                if (isSelected)
                {
                    SDL_StopTextInput();
                    isSelected = false;
                }
            }
        }
        else if (pInput->isTextInput() && isSelected)
        {
            std::string txt = pInput->getTextInput();
            std::string tmpText = text.substr(0, cursorPosition) + txt;
            if (text.size() > cursorPosition)
                tmpText += text.substr(cursorPosition);
            std::istringstream us_in(tmpText);
            us_in.imbue(Localisation::Instance().getLocale());

            us_in >> value;

            bool hasFailed = us_in.fail();

            if (us_in.eof() && !hasFailed)
            {
                text = utils::string_format("%.2f", value);
                cursorPosition++;
                fireFuncionCall("valueChanged", value);
            }
            eventHandled = true;
        }
        else if (pInput->isKeyDown(SDLK_BACKSPACE) && isSelected)
        {
            if (text.size() > 0)
            {
                text = text.substr(0, cursorPosition - 1) + text.substr(cursorPosition);
                value = std::atof(text.c_str());

                cursorPosition--;
                fireFuncionCall("valueChanged", value);
            }
            eventHandled = true;
        }
        else if (pInput->isKeyDown(SDLK_LEFT))
        {
            if (cursorPosition == 0)
            {
                return eventHandled;
            }
            cursorPosition--;
            eventHandled = true;
        }
        else if (pInput->isKeyDown(SDLK_RIGHT))
        {
            if (cursorPosition == text.size())
            {
                return eventHandled;
            }
            cursorPosition++;
            eventHandled = true;
        }
        return eventHandled;
    }

    void NumberTextItem::setColor(int r, int g, int b)
    {
        color.r = r;
        color.g = g;
        color.b = b;
    }
}