/*
 * Label.cpp
 *
 *  Created on: 29.05.2016
 *      Author: stefan
 */

#include "engine/ui/Label.h"
#include "engine/graphics/TextureManager.h"
namespace UI
{

    Label::Label(const std::string &text, Object *parent)
        : UI::Object(parent)
    {
        setObjectName("label");
        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }
        color = {255, 255, 255, 255};
        color = getTheme()->getStyleColor(this, UI::StyleType::Color);
        std::string fontName = getTheme()->getStyleText(this, UI::StyleType::FontName);
        int fontSize = getTheme()->getStyleInt(this, UI::StyleType::FontSize);
        if (!fontName.empty())
        {
            setFont(fontName, fontSize);
        }
        setText(text);
    }
    Label::Label(Object *parent)
        : UI::Object(parent), text("")
    {
        color = {255, 255, 255, 255};
    }

    void Label::render(core::Renderer *pRender)
    {
        int x = getX();
        int y = getY();
        if (getParent() != nullptr)
        {
            graphics::Rect rect = getParent()->displayRect();
            x += rect.x;
            y += rect.y;
        }
        auto oldStyle = getFont()->getStyle();
        getFont()->setStyle(m_fontStyle);
        getFont()->render(pRender, text, color, x, y);
        getFont()->setStyle(oldStyle);
    }

    std::string Label::getText()
    {
        return text;
    }
    void Label::setText(const std::string &text)
    {
        this->text = text;

        if (getFont() != nullptr)
        {
            int w, h = 0;
            getFont()->size(text, &w, &h);
            setWidth(w);
            setHeight(h);
        }
    }

    void Label::setColor(SDL_Color color)
    {
        this->color = color;
    }

    Label::~Label()
    {
        // TODO Auto-generated destructor stub
    }

    void Label::setStyle(graphics::FontStyle fontStyle)
    {
        m_fontStyle = fontStyle;
    }

} /* namespace UI */
