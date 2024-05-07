/*
 * TabBar.cpp
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#include <engine/graphics/TextureManager.h>
#include <engine/ui/TabBar.h>

namespace UI
{

    TabBar::TabBar(Object *parent, TabDirection tabDirection) : UI::Object(parent), currentTab(-1), hoveredTab(-1), tabDirection(tabDirection)
    {
        setObjectName("tabbar");

        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }
        auto textureName = getTheme()->getStyleText(this, UI::StyleType::BackgroundImage);
        btnTexture = nullptr;
        if (!textureName.empty())
            btnTexture = graphics::TextureManager::Instance().loadTexture(textureName);
        color = getTheme()->getStyleColor(this, UI::StyleType::Color);

        disabledColor = getTheme()->getStyleColor(this, UI::StyleType::DisabledColor);

        hoverColor = getTheme()->getStyleColor(this, UI::StyleType::HoverColor);
        backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
    }

    TabBar::~TabBar()
    {
        // TODO Auto-generated destructor stub
    }
    void TabBar::render(core::Renderer *renderer)
    {
        // render tabbar
        graphics::Rect displayRect;
        if (getParent() != nullptr)
        {
            displayRect = getParent()->displayRect();
        }
        int index = 0;
        SDL_Color currentColor = {198, 197, 197, 255};
        int tabX = static_cast<int>(displayRect.x) + 16;
        int taby = static_cast<int>(displayRect.y) + 16;

        for (auto &tab : tabs)
        {
            const std::string &title = tab->getTitle();
            // render tab
            int w = 0;
            int h = 0;
            getFont()->size(title, &w, &h);

            graphics::Rect backgroundRect;
            calulateTabRect(index, backgroundRect);

            renderer->setDrawColor(backgroundColor);
            renderer->fillRect(backgroundRect);

            if (index == hoveredTab)
            {
                currentColor = hoverColor;
            }
            else if (index == currentTab)
            {
                currentColor = color;
            }
            else
            {
                currentColor = disabledColor;
            }

            if (btnTexture != nullptr)
            {
                // left top corner

                btnTexture->render(renderer, tabX, taby, 9, 9, 0, 0);
                // left bottom corner
                btnTexture->render(renderer, tabX, taby + tabHeight - 9, 9, 9, 0, 20);

                btnTexture->render(renderer, tabX + tabWidth + 25 - 9, taby, 9, 9, 174, 0);
                btnTexture->render(renderer, tabX + tabWidth + 25 - 9, taby + tabHeight - 9, 9, 9, 174, 20);
            }
            else
            {
                renderer->setDrawColor(getTheme()->getStyleColor(this, UI::StyleType::BorderColor));
                renderer->drawRect(backgroundRect);
            }

            // tabX += 10; // offset
            // render title
            getFont()->render(renderer, title, currentColor, backgroundRect.x + 10, backgroundRect.y + 5);
            taby += tabHeight + 10;
            index++;
        }

        renderer->setDrawColor(93, 103, 108, 255);
        if (tabDirection == TabDirection::Left)
        {
            utils::Vector2 start(displayRect.x + getX() + 183, displayRect.y + getY() + 16);
            utils::Vector2 end(displayRect.x + getX() + 183, displayRect.y + getY() + getHeight() - 16);
            renderer->drawLine(start, end);
        }
        else
        {
            utils::Vector2 start(displayRect.x + getX() + 16, displayRect.y + getY() + 60);
            utils::Vector2 end(displayRect.x + getX() + getWidth() + getY() - 16, displayRect.y + 60);
            renderer->drawLine(start, end);
        }

        if (currentTab != -1)
        {
            tabs[currentTab]->render(renderer);
        }
    }

    void TabBar::renderLeft([[maybe_unused]] core::Renderer *renderer) {}

    void TabBar::renderTop([[maybe_unused]] core::Renderer *renderer) {}

    void TabBar::postRender(core::Renderer *renderer)
    {
        if (currentTab != -1)
        {
            tabs[currentTab]->postRender(renderer);
        }
        UI::Object::postRender(renderer);
    }

    void TabBar::calulateTabRect([[maybe_unused]] size_t index, graphics::Rect &rect)
    {
        graphics::Rect displayRect;
        if (getParent() != nullptr)
        {
            displayRect = getParent()->displayRect();
        }
        int tabX = static_cast<int>(displayRect.x) + 16 + getX();
        int taby = static_cast<int>(displayRect.y) + 16 + getY();
        if (tabDirection == TabDirection::Left)
        {
            rect.x = tabX;
            rect.y = taby + ((tabHeight + 10) * index);
        }
        else
        {
            rect.x = tabX + ((tabWidth + 20) * index);
            rect.y = taby;
        }

        rect.width = tabWidth + 20;
        rect.height = tabHeight + 6;
    }

    bool TabBar::handleEvents(core::Input *pInput)
    {
        bool eventHandled = false;
        if (currentTab != -1)
        {
            eventHandled = tabs[currentTab]->handleEvents(pInput);
            // handle tabbar events
        }

        hoveredTab = -1;
        graphics::Rect displayRect;
        if (getParent() != nullptr)
        {
            displayRect = getParent()->displayRect();
        }

        graphics::Rect tabRect;
        int index = 0;
        for (auto &tab : tabs)
        {
            std::string title = tab->getTitle();

            // render background
            calulateTabRect(index, tabRect);

            if (tabRect.intersects(pInput->getMousePostion()))
            {
                if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
                {
                    setActiveTab(index);
                    eventHandled = true;
                }
                // hover
                hoveredTab = index;
            }

            index++;
        }
        UI::Object::handleEvents(pInput);
        return eventHandled;
    }

    void TabBar::addTab(std::shared_ptr<Tab> tab)
    {
        tabs.push_back(tab);
        if (currentTab == -1)
            setActiveTab(0);
    }
    void TabBar::removeTabByIndex(int index)
    {
        auto it = tabs.begin() + index;
        tabs.erase(it);
    }
    void TabBar::removeTab(std::shared_ptr<Tab> tab)
    {
        auto it = std::find(tabs.begin(), tabs.end(), tab);
        if (it != tabs.end())
        {
            tabs.erase(it);
            setActiveTab(0);
        }
    }

    graphics::Rect TabBar::displayRect()
    {
        graphics::Rect r;
        graphics::Rect tabRect;
        calulateTabRect(0, tabRect);

        if (tabDirection == TabDirection::Left)
        {
            r.x = getX() + 183;
            r.y = getY() + 20;
        }
        else
        {
            r.x = getX() + 16;
            r.y = getY() + 60;
        }
        if (getParent() != nullptr)
        {
            r.x += getParent()->displayRect().x;
            r.y += getParent()->displayRect().y;
        }
        return r;
    }

    graphics::Rect TabBar::eventRect()
    {
        graphics::Rect r;
        if (tabDirection == TabDirection::Left)
        {
            r.x = getX() + 183;
            r.y = getY() + 20;
        }
        else
        {
            r.x = getX() + 16;
            r.y = getY() + 60;
        }
        if (getParent() != nullptr)
        {
            r.x += getParent()->eventRect().x;
            r.y += getParent()->eventRect().y;
        }
        return r;
    }

    void TabBar::setCurrentTab(std::shared_ptr<Tab> tab)
    {
        auto it = std::find(tabs.begin(), tabs.end(), tab);
        setActiveTab(std::distance(tabs.begin(), it));
    }

    void TabBar::setTabWidth(int width) { tabWidth = width; }

    void TabBar::setVisible(bool visible)
    {
        Object::setVisible(visible);
        tabs[currentTab]->setVisible(isVisible());
    }

    void TabBar::setActiveTab(int index)
    {
        if (currentTab >= 0 && currentTab < static_cast<int>(tabs.size()))
            tabs[currentTab]->setVisible(false);
        currentTab = index;
        tabs[currentTab]->setVisible(isVisible());
    }
} /* namespace UI */
