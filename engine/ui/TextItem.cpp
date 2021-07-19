/*
 * TextItem.cpp
 *
 *  Created on: 05.02.2017
 *      Author: stefan
 */

#include <engine/ui/TextItem.h>

namespace UI
{

    TextItem::TextItem(Object *parent, int pWidth, int pHeight)
        : UI::Object(parent, pWidth, pHeight), isSelected(false)
    {

        // TODO Auto-generated constructor stub
        SDL_StopTextInput();
        color = {255, 255, 255, 255};
        cursorPosition = 0;
    }

    TextItem::~TextItem()
    {
        // TODO Auto-generated destructor stub
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

        pRender->setDrawColor(12, 21, 24, 255);

        pRender->fillRect(rect);
        pRender->setDrawColor(93, 103, 108, 255);
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
    void TextItem::handleEvents(core::Input *pInput)
    {
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            if (displayRect().intersects(pInput->getMousePostion()))
            {
                SDL_Rect rect = displayRect().sdlRect();
                SDL_SetTextInputRect(&rect);
                SDL_StartTextInput();
                isSelected = true;
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
            text = text.substr(0, cursorPosition) + txt + text.substr(cursorPosition);
            cursorPosition++;
            fireFuncionCall("textChanged", text);
        }
        else if (pInput->isKeyDown(SDLK_BACKSPACE) && isSelected)
        {
            if (text.size() > 0)
            {
                text = text.substr(0, cursorPosition - 1) + text.substr(cursorPosition);
                cursorPosition--;
                fireFuncionCall("textChanged", text);
            }
        }
        else if (pInput->isKeyDown(SDLK_LEFT))
        {
            if (cursorPosition == 0)
            {
                return;
            }
            cursorPosition--;
        }
        else if (pInput->isKeyDown(SDLK_RIGHT))
        {
            if (cursorPosition == text.size())
            {
                return;
            }
            cursorPosition++;
        }
    }
    void TextItem::setFont(const std::string &fontname, unsigned int font_size)
    {
        graphics::Text *text = new graphics::Text();
        text->openFont(fontname, font_size);
        Object::setFont(text);
    }
    void TextItem::setColor(int r, int g, int b)
    {
        color.r = r;
        color.g = g;
        color.b = b;
    }
} /* namespace UI */
