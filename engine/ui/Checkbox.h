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
        explicit Checkbox(Object *parent);
        /** Default destructor */
        ~Checkbox() override;
        [[nodiscard]] bool isChecked() const { return checked; }
        void setChecked(bool check);
        void toggleChecked();
        void setText(const std::string &text) { this->text = text; }
        void render(core::Renderer *pRender) override;
        bool handleEvents(core::Input *pInput) override;

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
