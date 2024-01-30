#include "engine/ui/scrollarea.h"
#include <algorithm>
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/logger.h>

namespace UI
{

    ScrollArea::ScrollArea(const unsigned int pWidth, const unsigned pHeight, Object *parent = 0) :
        Object(parent), scrollWidth(0), scrollHeight(0), scrollX(0), scrollY(0), scrollPosX(0), scrollPosY(0),
        buttonPressed(false)
    {
        setObjectName("scrollarea");
        renderRect.x = 0;
        renderRect.y = 0;
        renderRect.width = pWidth;
        renderRect.height = pHeight;
        renderArea = nullptr;
        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }
        backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
        foregroundColor = getTheme()->getStyleColor(this, UI::StyleType::ForgroundColor);

        uiText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 12);
    }

    ScrollArea::~ScrollArea()
    {
        // dtor
        delete renderArea;
    }

    void ScrollArea::reset(bool noscrollreset)
    {
        if (!noscrollreset)
        {
            scrollPosX = 0;
            scrollPosY = 0;
        }
        delete renderArea;
        renderArea = nullptr;
    }

    void ScrollArea::render(core::Renderer *pRender)
    {
        graphics::Rect parentRect;
        if (getParent() != nullptr)
            parentRect = getParent()->displayRect();

        // render scroolbar
        // 82 354
        const float lastScrollWidth = scrollWidth;
        const float lastScrollHeight = scrollHeight;
        scrollWidth = 0;
        scrollX = 0;
        for (const auto &object: objects)
        {
            const graphics::Rect r = object->displayRect();
            if (scrollWidth < r.x + r.width)
            {
                scrollWidth = r.x + r.width;
                scrollX = r.x;
            }

            if (scrollHeight < r.y + r.height)
            {
                scrollHeight = r.y + r.height;
                scrollY = r.y;
            }
        }
        if (scrollWidth == 0)
            scrollWidth = renderRect.width;
        if (scrollHeight == 0)
            scrollHeight = renderRect.height;

        if (lastScrollWidth != scrollWidth || lastScrollHeight != scrollHeight)
        {
            delete renderArea;
            renderArea = nullptr;
        }

        // render Objects to Texture
        if (renderArea == nullptr)
        {

            renderArea = new graphics::Texture(pRender, scrollWidth, scrollHeight);

            pRender->setRenderTarget(renderArea);

            pRender->setDrawColor(backgroundColor);
            pRender->clear();

            for (auto &o: objects)
            {
                o->render(pRender);
            }
            pRender->setRenderTarget(nullptr);
        }
        graphics::Rect srcRect;
        graphics::Rect destRect;

        srcRect.width = renderArea->getWidth();
        srcRect.height = renderArea->getHeight();
        srcRect.x = scrollPosX;
        srcRect.y = scrollPosY;
        if (renderArea->getWidth() < renderRect.width)
        {
            destRect.width = renderArea->getWidth();
        }
        else
        {
            destRect.width = renderRect.width;
            srcRect.width = renderRect.width;
        }
        if (renderArea->getHeight() < renderRect.height)
        {
            destRect.height = renderArea->getHeight();
        }
        else
        {
            srcRect.height = renderRect.height;
            destRect.height = renderRect.height;
        }
        destRect.x = parentRect.x + getX();
        destRect.y = parentRect.y + getY();

        renderArea->render(pRender, srcRect, destRect);

        pRender->setDrawColor(93, 103, 108, 255);
        pRender->setDrawColor(foregroundColor);
        graphics::Rect borderRect;
        borderRect.x = parentRect.x + getX();
        borderRect.y = parentRect.y + getY();
        borderRect.width = renderRect.width;
        borderRect.height = renderRect.height;

        pRender->drawRect(borderRect);

        if (renderArea->getHeight() > renderRect.height)
        {
            graphics::Rect scrollbarRect;
            scrollbarRect.width = 10;
            scrollbarRect.x = parentRect.x + getX() + renderRect.width - scrollbarRect.width - 1;
            scrollbarRect.y = parentRect.y + getY() + 2;

            scrollbarRect.height = renderRect.height - 2;

            pRender->drawRect(scrollbarRect);

            SDL_Color uiColor = foregroundColor;
            uiText->render(pRender, "\uf0d7", uiColor, scrollbarRect.x + 2,
                           scrollbarRect.y + scrollbarRect.height - 14);

            uiText->render(pRender, "\uf0d8", uiColor, scrollbarRect.x + 2, scrollbarRect.y + 2);

            scrollbarRect.x += 2;
            float relativeHeight = (renderRect.height - 26) / renderArea->getHeight();
            scrollbarRect.y += (relativeHeight * scrollPosY) + 16;
            scrollbarRect.height = (relativeHeight * (renderRect.height - 16));
            scrollbarRect.width -= 4;
            pRender->fillRect(scrollbarRect);
        }
    }

    void ScrollArea::postRender(core::Renderer *renderer) { Container::postRender(renderer); }

    graphics::Rect ScrollArea::displayRect() { return renderRect; }

    graphics::Rect ScrollArea::eventRect()
    {
        graphics::Rect r;
        r.x = renderRect.x + getX() - scrollPosX;
        r.y = renderRect.y + getY() - scrollPosY;
        r.width = renderRect.width;
        r.height = renderRect.height;
        if (getParent() != nullptr)
        {
            r.x += getParent()->eventRect().x;
            r.y += getParent()->eventRect().y;
        }
        return r;
    }

    bool ScrollArea::handleEvents(core::Input *pInput)
    {
        reset();
        bool eventHandled = Container::handleEvents(pInput);
        int i = 0;

        graphics::Rect parentRect;
        if (getParent() != nullptr)
            parentRect = getParent()->displayRect();

        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            for (std::shared_ptr<Object> o: objects)
            {
                graphics::Rect r = o->displayRect();
                r.x -= scrollPosX;
                r.x += getX() + parentRect.x;
                r.y -= scrollPosY;
                r.y += getY() + parentRect.y;
                if (r.intersects(pInput->getMousePostion()))
                {
                    fireFuncionCall("selectItem", i);
                    break;
                }
                i++;
            }
            buttonPressed = true;
        }
        else if (pInput->isMouseButtonUp(SDL_BUTTON_LEFT))
        {
            buttonPressed = false;
        }
        if (buttonPressed)
        {
            graphics::Rect destRect;

            destRect.width = 14;
            destRect.height = 14;
            destRect.x = parentRect.x + getX();
            destRect.y = parentRect.y + renderRect.height + getY();

            if (destRect.intersects(pInput->getMousePostion()))
            {
                SGL_LOG_TRACE(" left edge clicked");
            }

            // render right edge
            destRect.x = parentRect.x + renderRect.width - 14 + getX();
            if (destRect.intersects(pInput->getMousePostion()))
            {
                SGL_LOG_TRACE(" right edge clicked");
            }
            // render bar
            destRect.width = renderRect.width - 28;
            destRect.height = 5;
            destRect.x = parentRect.x + 14 + getX();
            destRect.y = parentRect.y + renderRect.height + 4 + getY();
            if (destRect.intersects(pInput->getMousePostion()))
            {
                SGL_LOG_TRACE(" bar clicked");
            }

            // check horizontal

            destRect.width = 14;
            destRect.height = 14;
            destRect.x = parentRect.x + getX() + renderRect.width - 14;
            destRect.y = parentRect.y + getY();
            if (destRect.intersects(pInput->getMousePostion()))
            {
                if (scrollPosY > 0)
                {
                    scrollPosY -= std::round((scrollHeight - renderRect.height - 14) / 100);
                    if (scrollPosY < 0)
                        scrollPosY = 0;

                    eventHandled = true;
                }
            }
            // render right edge
            destRect.x = parentRect.x + renderRect.width - 14 + getX();
            destRect.y = parentRect.y + renderRect.height + getY() - 14;
            if (destRect.intersects(pInput->getMousePostion()))
            {
                if (scrollPosY < scrollHeight - renderRect.height - 14)
                    scrollPosY += std::round((scrollHeight - renderRect.height - 14) / 100);
                eventHandled = true;
            }
            // render bar

            destRect.width = 4;
            destRect.height = renderRect.height - 24;
            destRect.x = parentRect.x + renderRect.width - 8 + getX();
            destRect.y = parentRect.y + 14 + getY();
            if (destRect.intersects(pInput->getMousePostion()))
            {
                SGL_LOG_TRACE(" bar 2 clicked");
            }
            // render slider
            destRect.width = 14;
            destRect.height = 10;
            destRect.x = parentRect.x + renderRect.width - 14 + getX();
            destRect.y = parentRect.y + 14 + getY();
        }

        graphics::Rect r = renderRect;
        r.x = getX() + parentRect.x;
        r.y = getY() + parentRect.y;
        if (pInput->isScrollWheel())
        {
            if (!r.intersects(pInput->getMousePostion()))
                return eventHandled;
            utils::Vector2 pos = pInput->getMouseWheelPosition();
            if (scrollWidth - renderRect.width > 0)
            {
                if ((scrollPosX + (pos.getY() * -5) >= 0 && pos.getY() == 1) ||
                    (pos.getY() == -1 && scrollPosX < scrollWidth - renderRect.width - 14))
                {
                    scrollPosX += pos.getY() * -5;
                    eventHandled = true;
                }
            }
            else
            {
                if ((scrollPosY + (pos.getY() * -5) >= 0 && pos.getY() == 1) ||
                    (pos.getY() == -1 && scrollPosY < scrollHeight - renderRect.height - 14))
                {
                    scrollPosY += pos.getY() * -5;
                    eventHandled = true;
                }
            }
        }
        return eventHandled;
    }

} // namespace UI
