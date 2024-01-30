#ifndef UI_SCROLLAREA_H
#define UI_SCROLLAREA_H

#include <engine/ui/Container.h>
#include <engine/ui/Object.h>

namespace UI
{

    class ScrollArea : public UI::Container, public UI::Object
    {
    public:
        /** Default constructor */
        ScrollArea(const unsigned int pWidth, const unsigned pHeight,
                   Object *parent);
        /** Default destructor */
        ~ScrollArea() override;
        void render(core::Renderer *pRender) override;
        void postRender(core::Renderer *renderer) override;
        graphics::Rect displayRect() override;
        graphics::Rect eventRect() override;
        bool handleEvents(core::Input *pInput) override;
        void reset(bool noscrollreset = true);

    protected:
    private:
        graphics::Rect renderRect;
        float scrollWidth, scrollHeight;
        float scrollX, scrollY;
        float scrollPosX, scrollPosY;
        graphics::Texture *renderArea;
        bool buttonPressed;
        std::shared_ptr<graphics::Text> uiText;
        SDL_Color backgroundColor;
        SDL_Color foregroundColor;
    };

} // namespace UI

#endif // UI_SCROLLAREA_H
