#include "engine/ui/Window.h"
#include "engine/ui/Theme.h"
#include <functional>
#include <engine/graphics/TextureManager.h>
namespace UI
{

    int Window::windowCount = 0;

    Window::Window()
        : Window(50, 50, 300, 400)
    {
        //addObject()
    }

    Window::Window(int x, int y, int width, int height)
        : width(width), height(height)
    {
        setObjectName("window");
        setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        setX(x);
        setY(y);
        visible = false;
        windowCount++;

        closeButton = std::make_shared<UI::Button>(this);
        closeButton->setX(width - 23 - 5 - 16);
        closeButton->setY(7 - 40);
        std::string iconFontName = getTheme()->getStyleText(this, UI::StyleType::IconFontName);
        int iconFontSize = getTheme()->getStyleInt(this, UI::StyleType::IconFontSize);

        closeButton->setFont(iconFontName, iconFontSize);
        closeButton->setLabel("\uf00d");
        closeButton->setBorderless(true);
        SDL_Color buttonColor = getTheme()->getStyleColor(this, UI::StyleType::TitleColor); //{195, 129, 42, 255};
        closeButton->setColor(buttonColor);

        this->addObject(closeButton);
        closeButton->connect("buttonClick", [&]()
                             { buttonClick(); });
        title = "Demo";
    }

    void Window::handleEvents(core::Input *pInput)
    {
        if (visible)
            UI::Container::handleEvents(pInput);
    }

    void Window::buttonClick()
    {
        this->setVisible(false);
    }

    void Window::render(core::Renderer *pRender)
    {
        if (visible)
        {
            SDL_Color titleColor = getTheme()->getStyleColor(this, UI::StyleType::TitleColor);

            //draw top
            graphics::Rect topRect;
            topRect.x = getX();
            topRect.y = getY();
            topRect.width = width;
            topRect.height = 2;
            pRender->setDrawColor(titleColor);
            pRender->fillRect(topRect);
            pRender->drawRect(topRect);

            graphics::Rect backgroundRect;
            backgroundRect.x = getX();
            backgroundRect.y = getY() + topRect.height;
            backgroundRect.width = width;
            backgroundRect.height = height - topRect.height;
            pRender->setDrawColor(getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor));

            pRender->fillRect(backgroundRect);
            pRender->drawRect(backgroundRect);
            //draw title
            getFont()->render(pRender, title, titleColor, backgroundRect.x + 16, backgroundRect.y + 5);

            pRender->setDrawColor(getTheme()->getStyleColor(this, UI::StyleType::BorderColor));
            utils::Vector2 start(backgroundRect.x + 16, getY() + 29);
            utils::Vector2 end(backgroundRect.x + width - 16, getY() + 29);
            pRender->drawLine(start, end);

            UI::Container::render(pRender);
        }
    }

    Window::~Window()
    {
        UI::Container::clear();
        closeButton = nullptr;
    }

    graphics::Rect Window::displayRect()
    {
        graphics::Rect r;
        r.x = getX() + 16;
        r.y = getY() + 30;
        r.width = width - 5;
        r.height = height - 40;
        return r;
    }

    graphics::Rect Window::eventRect()
    {
        return displayRect();
    }

    void Window::clear()
    {
        UI::Container::clear();
        addObject(closeButton);
    }

} // namespace UI
