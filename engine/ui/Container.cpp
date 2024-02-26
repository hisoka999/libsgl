#include "engine/ui/Container.h"
#include <algorithm>
#include "engine/ui/layout/Layout.h"

namespace UI
{

    Container::Container()
    {
        // ctor
    }

    Container::~Container() {}

    void Container::addObject(const std::shared_ptr<Object> &obj)
    {
        objects.push_back(obj);

        // std::sort(std::begin(objects), std::end(objects), [](const std::shared_ptr<Object> &a, const
        // std::shared_ptr<Object> &b) -> bool {
        //     return a->getRenderOrder() < b->getRenderOrder();
        // });
    }
    void Container::removeObject(const std::shared_ptr<Object> &obj)
    {
        auto it = std::find(objects.begin(), objects.end(), obj);
        if (it != objects.end())
            objects.erase(it);
    }
    void Container::updateLayout()
    {
        if (m_layout)
        {
            graphics::Rect layoutRect;
            boundsRect(layoutRect);
            m_layout->updateLayout(layoutRect);
        }
    }

    void Container::render(core::Renderer *pRender)
    {
        if (needRefresh)
        {
            refresh();
            updateLayout();
        }
        for (const auto &obj: objects)
        {
            if (obj != nullptr)
                obj->render(pRender);
        }
    }

    void Container::postRender(core::Renderer *pRender)
    {
        for (const auto &obj: objects)
        {
            if (obj != nullptr)
                obj->postRender(pRender);
        }
    }
    bool Container::handleEvents(core::Input *pInput)
    {
        bool eventHandled = false;
        for (auto it = objects.rbegin(); it != objects.rend(); it++)
        // for (const auto &obj : objects)
        {
            auto &obj = *it;
            if (obj != nullptr)
            {
                eventHandled = obj->handleEvents(pInput);
                if (eventHandled)
                    break;
            }
        }
        return eventHandled;
    }

    size_t Container::size() { return objects.size(); }

    std::shared_ptr<Object> Container::get(size_t pos) { return objects.at(pos); }

    void Container::needsRefresh() { needRefresh = true; }

    void Container::setLayout(const std::shared_ptr<layout::Layout> &layout) { m_layout = layout; }

    void Container::refresh() { endRefresh(); }

    void Container::endRefresh() { needRefresh = false; }

    void Container::boundsRect(graphics::Rect &rect)
    {
        rect.x = 0;
        rect.y = 0;
        rect.width = 0;
        rect.height = 0;
        for (auto &object: objects)
        {
            if (object->getParent() != nullptr)
            {
                rect.x = std::max(rect.x, float(object->getX() + object->getParent()->getX()));
                rect.y = std::max(rect.y, float(object->getY() + object->getParent()->getY()));
                rect.width = std::max(rect.width, float(object->getWidth()));
                rect.height = std::max(rect.height, float(object->getHeight()));
            }
            rect.x = std::max(rect.x, float(object->getX()));
            rect.y = std::max(rect.y, float(object->getY()));
            rect.width = std::max(rect.x, float(object->getWidth()));
            rect.height = std::max(rect.x, float(object->getHeight()));
        }
    }
    void Container::clear() { objects.clear(); }

} // namespace UI
