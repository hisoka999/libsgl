#include "engine/ui/Checkbox.h"
#include <engine/graphics/TextureManager.h>

namespace UI
{

    Checkbox::Checkbox(Object *parent)
        : UI::Object(parent)
    {
        uiIconText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        //    textCore = new graphics::Text();
        //    textCore->openFont("arial.ttf", 12);
        checked = false;
        color = {255, 255, 255, 255};
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

    void Checkbox::handleEvents(core::Input *pInput)
    {
        int tx = getX();

        int ty = getY();

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
        rect.height = 25;
        int textWidth, textHeight = 0;
        getFont()->size(text, &textWidth, &textHeight);
        rect.width += textWidth;

        if (rect.intersects(pInput->getMousePostion()) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            std::cout << "toggle checked" << std::endl;
            toggleChecked();
        }
    }

    void Checkbox::render(core::Renderer *pRender)
    {

        int tx = getX();
        int ty = getY();
        if (this->getParent())
        {
            tx += this->getParent()->getX();
            ty += this->getParent()->getY();
        }
        int x2 = 0;
        if (checked)
            uiIconText->render(pRender, "\uf00c", color, tx, ty);
        //pTexture->render(pRender, tx, ty, 25, 23, x2, 260);
        graphics::Rect rect;
        rect.x = tx;
        rect.y = ty;
        rect.width = 25;
        rect.height = 25;

        pRender->drawRect(rect);
        getFont()->render(pRender, text, color, tx + 30, ty + 5);
    }

} // namespace UI
