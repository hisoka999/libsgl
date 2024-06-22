#ifndef UI_SCROLLAREA_H
#define UI_SCROLLAREA_H

#include <engine/ui/Container.h>
#include <engine/ui/Object.h>
#include <engine/ui/VerticalScrollbar.h>
namespace UI
{

    class ScrollArea : public UI::Container, public UI::Object
    {
    public:
        /** Default constructor */
        ScrollArea(const unsigned int pWidth, const unsigned pHeight, Object *parent);
        /** Default destructor */
        ~ScrollArea() override;
        void render(core::Renderer *pRender) override;
        void postRender(core::Renderer *renderer) override;
        graphics::Rect displayRect() override;
        graphics::Rect eventRect() override;
        bool handleEvents(core::Input *pInput) override;
        void reset(bool noscrollreset = true);
        void setX(int x) override;
        void setY(int y) override;
        void setPos(int x, int y) override;

    protected:
    private:
        graphics::Rect renderRect;
        float scrollWidth, scrollHeight;
        float scrollPosX, scrollPosY;
        std::unique_ptr<graphics::Texture> renderArea;
        bool buttonPressed;
        SDL_Color backgroundColor;
        SDL_Color foregroundColor;
        VerticalScrollbar virticalScrollBar;
    };

} // namespace UI

#endif // UI_SCROLLAREA_H
