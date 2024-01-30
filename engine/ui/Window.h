#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/ui/Button.h"
#include "engine/ui/Container.h"
#include "engine/ui/Object.h"

namespace UI
{

    class Window : public Container, public Object
    {
    public:
        /** Default constructor */
        Window();
        Window(int x, int y, int width, int height);
        /** Default destructor */
        ~Window() override;

        void render(core::Renderer *pRender) override;
        void postRender(core::Renderer *renderer) override;
        bool handleEvents(core::Input *pInput) override;
        bool handleWindowEvents(core::Input *pInput);
        void setVisible(bool visible);
        bool getVisible();

        void setTitle(std::string title);

        graphics::Rect displayRect() override;
        graphics::Rect eventRect() override;

        void setSize(int width, int height);
        void clear() override;
        int getWidth() override;
        int getHeight() override;
        void makeActive();
        void makeInactive();
        bool isActive() const;

    protected:
        virtual void onOpen() { needsRefresh(); };
        void setWithoutTitle(bool withoutTitle);
        virtual void boundsRect(graphics::Rect &rect) override;
        void setActive(bool value);

    private:
        int m_width;
        int m_height;
        bool m_visible = false;
        size_t m_is_closing = 0;
        virtual void buttonClick();
        std::shared_ptr<UI::Button> m_closeButton;
        std::string m_title;
        bool m_withoutTitle = false;
        bool m_active = false;
    };

} // namespace UI

#endif // UI_WINDOW_H
