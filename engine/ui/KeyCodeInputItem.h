#pragma once

#include "Object.h"

namespace UI
{
    class KeyCodeInputItem : public UI::Object
    {
    private:
        SDL_Keycode m_keyCode;
        SDL_Color color;
        bool isSelected;
        SDL_Color borderColor;
        SDL_Color backgroundColor;
        SDL_Color hoverColor;

    public:
        KeyCodeInputItem(Object *parent, int pWidth, int pHeight);
        ~KeyCodeInputItem();

        SDL_Keycode getValue();
        void setValue(const SDL_Keycode pValue);

        void render(core::Renderer *pRender);
        bool handleEvents(core::Input *pInput);
        void setColor(int r, int g, int b);
    };

} // namespace UI
