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
        virtual ~ScrollArea();
        virtual void render(core::Renderer *pRender);
        virtual void postRender(core::Renderer *renderer);
        virtual graphics::Rect displayRect();
        virtual graphics::Rect eventRect();
        virtual bool handleEvents(core::Input *pInput);
        void reset(bool noscrollreset = true);

    protected:
    private:
        graphics::Rect renderRect;
        int scrollWidth, scrollHeight;
        int scrollX, scrollY;
        int scrollPosX, scrollPosY;
        graphics::Texture *renderArea;
        bool buttonPressed;
        std::shared_ptr<graphics::Text> uiText;
        SDL_Color backgroundColor;
        SDL_Color foregroundColor;
    };

} // namespace UI

#endif // UI_SCROLLAREA_H
