#include "engine/ui/Checkbox.h"
#include <engine/graphics/TextureManager.h>

namespace UI
{

    Checkbox::Checkbox(Object *parent)
        : UI::Object(parent)
    {
        setObjectName("checkbox");

        checked = false;
        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }
        setHeight(25);

        color = getTheme()->getStyleColor(this, UI::StyleType::Color);
        borderColor = getTheme()->getStyleColor(this, UI::StyleType::BorderColor);
        backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
        uiIconText = graphics::TextureManager::Instance().loadFont(getTheme()->getStyleText(this, UI::StyleType::IconFontName), getTheme()->getStyleInt(this, UI::StyleType::IconFontSize));
    }

    Checkbox::~Checkbox()
    {
        uiIconText = nullptr;
    }

    void Checkbox::setChecked(bool check)
    {
        checked = check;
        fireFuncionCall("checkboxChanged", checked);
    }

    void Checkbox::toggleChecked()
    {
        setChecked(!checked);
    }

    bool Checkbox::handleEvents(core::Input *pInput)
    {
        float tx = float(getX());

        float ty = float(getY());

        if (this->getParent())
        {
            graphics::Rect dsp = getParent()->eventRect();
            tx += dsp.x;
            ty += dsp.y;
        }

        graphics::Rect rect;
        rect.x = tx;
        rect.y = ty;
        rect.width = 25;
        rect.height = getHeight();
        int textWidth, textHeight = 0;
        getFont()->size(text, &textWidth, &textHeight);
        rect.width += textWidth;

        if (rect.intersects(pInput->getMousePostion()) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            std::cout << "toggle checked" << std::endl;
            toggleChecked();
            return true;
        }
        return false;
    }

    void Checkbox::render(core::Renderer *pRender)
    {

        graphics::Rect rect = displayRect();

        rect.width = 25;
        rect.height = float(getHeight());
        pRender->setDrawColor(backgroundColor);
        pRender->fillRect(rect);
        pRender->setDrawColor(borderColor);
        pRender->drawRect(rect);

        int textWidth, textHeight = 0;
        getFont()->size(text, &textWidth, &textHeight);

        if (checked)
            uiIconText->render(pRender, "\uf00c", color, rect.x, rect.y + (getHeight() - textHeight) / 2);
        getFont()->render(pRender, text, color, rect.x + 30, rect.y + 5);
    }

} // namespace UI
