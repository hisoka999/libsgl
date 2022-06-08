#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H

#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <engine/ui/Object.h>
#include <iostream>

namespace UI
{

    class Checkbox : public UI::Object
    {
    public:
        /** Default constructor */
        Checkbox(Object *parent);
        /** Default destructor */
        virtual ~Checkbox();
        bool isChecked() { return checked; }
        void setChecked(bool check);
        void toggleChecked();
        void setText(std::string text) { this->text = text; }
        virtual void render(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);

    protected:
    private:
        bool checked;
        std::string text;
        std::shared_ptr<graphics::Text> uiIconText;
        SDL_Color color;
        SDL_Color borderColor;
        SDL_Color backgroundColor;
    };

} // namespace UI

#endif // UI_CHECKBOX_H
