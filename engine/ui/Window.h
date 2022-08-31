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
        virtual ~Window();

        virtual void render(core::Renderer *pRender);
        virtual void postRender(core::Renderer *renderer);
        virtual bool handleEvents(core::Input *pInput);
        void setVisible(bool visible);
        bool getVisible();

        void setTitle(std::string title);

        virtual graphics::Rect displayRect();
        virtual graphics::Rect eventRect();

        void setSize(int width, int height);
        virtual void clear();

    protected:
        virtual void onOpen()
        {
            needsRefresh();
        };
        void setWithoutTitle(bool withoutTitle);
        virtual void boundsRect(graphics::Rect &rect) override;

    private:
        int m_width;
        int m_height;
        bool m_visible = false;
        virtual void buttonClick();
        std::shared_ptr<UI::Button> m_closeButton;
        std::string m_title;
        bool m_withoutTitle = false;
    };

} // namespace UI

#endif // UI_WINDOW_H
