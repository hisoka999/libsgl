#ifndef __NUMBERTEXTITEM_H__
#define __NUMBERTEXTITEM_H__

#include <engine/ui/Object.h>

namespace UI
{
    class NumberTextItem : public UI::Object
    {
    private:
        /* data */
    public:
        NumberTextItem(Object *parent, int pWidth, int pHeight);
        ~NumberTextItem();

        float getValue();
        void setValue(const float pValue);

        void render(core::Renderer *pRender);
        bool handleEvents(core::Input *pInput);
        void setColor(int r, int g, int b);

    private:
        float value;
        std::string text;
        SDL_Color color;
        bool isSelected;
        size_t cursorPosition;
        SDL_Color borderColor;
        SDL_Color backgroundColor;
    };

} // namespace UI

#endif // __NUMBERTEXTITEM_H__