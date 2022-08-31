#include "engine/ui/Window.h"
#include "engine/ui/Theme.h"
#include <engine/graphics/TextureManager.h>
#include <functional>
namespace UI
{

    Window::Window()
        : Window(50, 50, 300, 400)
    {
        // addObject()
    }

    Window::Window(int x, int y, int width, int height)
        : m_width(width), m_height(height)
    {
        setObjectName("window");
        setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        setX(x);
        setY(y);
        m_visible = false;

        m_closeButton = std::make_shared<UI::Button>(this);
        m_closeButton->setX(width - 23 - 5 - 16);
        m_closeButton->setY(7 - 40);
        std::string iconFontName = getTheme()->getStyleText(this, UI::StyleType::IconFontName);
        int iconFontSize = getTheme()->getStyleInt(this, UI::StyleType::IconFontSize);

        m_closeButton->setFont(iconFontName, iconFontSize);
        m_closeButton->setLabel("\uf00d");
        m_closeButton->setBorderless(true);
        SDL_Color buttonColor = getTheme()->getStyleColor(this, UI::StyleType::TitleColor); //{195, 129, 42, 255};
        m_closeButton->setColor(buttonColor);

        m_closeButton->connect("buttonClick", [&]()
                               { buttonClick(); });
        m_title = "Demo";
    }

    bool Window::handleEvents(core::Input *pInput)
    {
        bool eventsHandled = false;
        if (m_visible)
        {
            eventsHandled = UI::Container::handleEvents(pInput);

            if (!eventsHandled)
                eventsHandled = displayRect().intersects(pInput->getMousePostion());

            if (!eventsHandled)
                eventsHandled = m_closeButton->handleEvents(pInput);
        }
        return eventsHandled;
    }

    void Window::setVisible(bool visible)
    {
        m_visible = visible;
        if (!m_visible)
        {
            this->fireFuncionCall("closed");
        }
        else
        {
            onOpen();
        }
    }

    bool Window::getVisible()
    {
        return m_visible;
    }

    void Window::setTitle(std::string title)
    {
        m_title = title;
    }

    void Window::buttonClick()
    {
        this->setVisible(false);
    }

    void Window::render(core::Renderer *pRender)
    {
        if (m_visible)
        {
            SDL_Color titleColor = getTheme()->getStyleColor(this, UI::StyleType::TitleColor);

            // draw top
            graphics::Rect topRect;
            topRect.x = getX();
            topRect.y = getY();
            topRect.width = m_width;
            topRect.height = 2;
            pRender->setDrawColor(titleColor);
            pRender->fillRect(topRect);
            pRender->drawRect(topRect);

            graphics::Rect backgroundRect;
            backgroundRect.x = getX();
            backgroundRect.y = getY() + topRect.height;
            backgroundRect.width = m_width;
            backgroundRect.height = m_height - topRect.height;
            pRender->setDrawColor(getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor));

            pRender->fillRect(backgroundRect);
            pRender->drawRect(backgroundRect);
            // draw title
            if (!m_withoutTitle)
            {
                getFont()->render(pRender, m_title, titleColor, backgroundRect.x + 16, backgroundRect.y + 5);

                pRender->setDrawColor(getTheme()->getStyleColor(this, UI::StyleType::BorderColor));
                utils::Vector2 start(backgroundRect.x + 16, getY() + 29);
                utils::Vector2 end(backgroundRect.x + m_width - 16, getY() + 29);
                pRender->drawLine(start, end);
                m_closeButton->render(pRender);
            }
            UI::Container::render(pRender);
        }
    }

    void Window::postRender(core::Renderer *renderer)
    {
        if (m_visible)
            UI::Container::postRender(renderer);
    }

    Window::~Window()
    {
        UI::Container::clear();
        m_closeButton = nullptr;
    }

    graphics::Rect Window::displayRect()
    {
        graphics::Rect r;
        r.x = getX() + 16;
        r.y = getY() + 30;
        r.width = m_width - 5;
        r.height = m_height - 40;
        return r;
    }

    graphics::Rect Window::eventRect()
    {
        return displayRect();
    }

    void Window::setSize(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    void Window::clear()
    {
        UI::Container::clear();
    }

    void Window::setWithoutTitle(bool withoutTitle)
    {
        m_withoutTitle = withoutTitle;
    }

    void Window::boundsRect(graphics::Rect &rect)
    {
        rect.x = 0;
        rect.y = 0;
        rect.width = m_width - 51;
        rect.height = m_height - 70;
    }

} // namespace UI
