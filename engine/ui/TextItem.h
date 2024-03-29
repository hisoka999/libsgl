/*
 * TextItem.h
 *
 *  Created on: 05.02.2017
 *      Author: stefan
 */

#ifndef UI_TEXTITEM_H_
#define UI_TEXTITEM_H_

#include <engine/ui/Object.h>

namespace UI
{

    class TextItem : public Object
    {
    public:
        TextItem(Object *parent, int pWidth, int pHeight);
        virtual ~TextItem();
        std::string getText();
        void setText(const std::string &text);

        void render(core::Renderer *pRender);
        bool handleEvents(core::Input *pInput);
        void setColor(int r, int g, int b);

    private:
        std::string text;
        SDL_Color color;
        bool isSelected;
        size_t cursorPosition;
        SDL_Color borderColor;
        SDL_Color backgroundColor;
        SDL_Color hoverColor;
    };

} /* namespace UI */

#endif /* UI_TEXTITEM_H_ */
