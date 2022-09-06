/*
 * TextItem.cpp
 *
 *  Created on: 05.02.2017
 *      Author: stefan
 */

#include <engine/ui/TextItem.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/string.h>
namespace UI
{

    std::string substr(std::string originalString, int maxLength)
    {
        std::string resultString = originalString;

        int len = 0;
        int byteCount = 0;

        const char *aStr = originalString.c_str();

        while (*aStr)
        {
            if ((*aStr & 0xc0) != 0x80)
                len += 1;

            if (len > maxLength)
            {
                resultString = resultString.substr(0, byteCount);
                break;
            }
            byteCount++;
            aStr++;
        }

        return resultString;
    }

    TextItem::TextItem(Object *parent, int pWidth, int pHeight)
        : UI::Object(parent, pWidth, pHeight), isSelected(false)
    {
        setObjectName("TextItem");
        SDL_StopTextInput();
        color = {255, 255, 255, 255};
        cursorPosition = 0;
        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }

        borderColor = getTheme()->getStyleColor(this, UI::StyleType::BorderColor);
        hoverColor = getTheme()->getStyleColor(this, UI::StyleType::HoverColor);

        backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
    }

    TextItem::~TextItem()
    {
    }

    std::string TextItem::getText()
    {
        return text;
    }
    void TextItem::setText(const std::string &text)
    {
        this->text = text;
        cursorPosition = text.size();
        fireFuncionCall("textChanged", text);
    }

    void TextItem::render(core::Renderer *pRender)
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
    bool TextItem::handleEvents(core::Input *pInput)
    {
        bool eventHandled = false;
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            if (eventRect().intersects(pInput->getMousePostion()))
            {
                pInput->startTextInput(eventRect());
                isSelected = true;
            }
            else
            {
                if (isSelected)
                {
                    pInput->stopTextInput();
                    isSelected = false;
                }
            }
        }
        else if (pInput->isTextInput() && isSelected)
        {
            std::string txt = pInput->getTextInput();
            text = text.substr(0, cursorPosition) + txt + text.substr(cursorPosition);
            cursorPosition += txt.length();
            fireFuncionCall("textChanged", text);
            eventHandled = true;
        }
        else if (pInput->isKeyDown(SDLK_BACKSPACE) && isSelected)
        {
            if (text.size() > 0)
            {
                int charLength = 0;
                int codePoint = -1;
                do
                {
                    charLength++;
                    std::string sub = text.substr(cursorPosition - charLength, charLength);

                    codePoint = utils::codepoint(sub);
                } while (codePoint == -1);

                text = text.substr(0, cursorPosition - charLength) + text.substr(cursorPosition);
                cursorPosition -= charLength;
                fireFuncionCall("textChanged", text);
                eventHandled = true;
            }
        }
        else if (pInput->isKeyDown(SDLK_DELETE))
        {
            if (text.size() > 0 && cursorPosition < text.size() - 1)
            {
                text = text.substr(0, cursorPosition) + text.substr(cursorPosition + 1);
                cursorPosition--;
                fireFuncionCall("textChanged", text);
                eventHandled = true;
            }
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
        else if (pInput->isKeyDown(SDLK_RETURN))
        {
            fireFuncionCall("inputSubmit", text);
            eventHandled = true;
        }
        return eventHandled;
    }

    void TextItem::setColor(int r, int g, int b)
    {
        color.r = r;
        color.g = g;
        color.b = b;
    }
} /* namespace UI */
