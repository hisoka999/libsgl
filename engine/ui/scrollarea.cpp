#include "engine/ui/scrollarea.h"
#include <algorithm>
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/logger.h>

namespace UI
{

    ScrollArea::ScrollArea(const unsigned int pWidth, const unsigned pHeight, Object *parent = 0) :
        Object(parent), scrollWidth(0), scrollHeight(0), scrollPosX(0), scrollPosY(0), buttonPressed(false),
        virticalScrollBar(parent)
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

        virticalScrollBar.setPos(renderRect.width, 0);
        virticalScrollBar.setHeight(pHeight);
    }

    ScrollArea::~ScrollArea() {}

    void ScrollArea::reset(bool noscrollreset)
    {
        if (!noscrollreset)
        {
            scrollPosX = 0;
            scrollPosY = 0;
        }
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
        scrollWidth = renderRect.width;
        scrollHeight = renderRect.height;
        for (const auto &object: objects)
        {
            const graphics::Rect objectRect = object->displayRect();
            scrollWidth = std::max(objectRect.x + objectRect.width, scrollWidth);
            scrollHeight = std::max(objectRect.y + objectRect.height, scrollHeight);
        }


        if (lastScrollWidth != scrollWidth || lastScrollHeight != scrollHeight)
        {
            renderArea = nullptr;
        }

        // render Objects to Texture
        if (renderArea == nullptr)
        {
            renderArea = std::make_unique<graphics::Texture>(pRender, scrollWidth, scrollHeight);
            pRender->setRenderTarget(renderArea.get());
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

        pRender->setDrawColor(foregroundColor);
        graphics::Rect borderRect;
        borderRect.x = parentRect.x + getX();
        borderRect.y = parentRect.y + getY();
        borderRect.width = renderRect.width;
        borderRect.height = renderRect.height;

        pRender->drawRect(borderRect);

        if (renderArea->getHeight() > renderRect.height)
        {
            virticalScrollBar.setContentHeight(renderArea->getHeight());
            virticalScrollBar.render(pRender);
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
        }


        virticalScrollBar.handleEvents(pInput);
        scrollPosY = virticalScrollBar.getScrollPosition();
        return eventHandled;
    }

} // namespace UI
