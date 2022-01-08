#include "engine/ui/Container.h"
#include <algorithm>

namespace UI
{

    Container::Container()
    {
        // ctor
    }

    Container::~Container()
    {
    }

    void Container::addObject(const std::shared_ptr<Object> &obj)
    {
        objects.push_back(obj);

        // std::sort(std::begin(objects), std::end(objects), [](const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) -> bool {
        //     return a->getRenderOrder() < b->getRenderOrder();
        // });
    }
    void Container::removeObject(const std::shared_ptr<Object> &obj)
    {
        auto it = std::find(objects.begin(), objects.end(), obj);
        if (it != objects.end())
            objects.erase(it);
    }
    void Container::render(core::Renderer *pRender)
    {
        if (needRefresh)
            refresh();
        for (const auto &obj : objects)
        {
            if (obj != nullptr)
                obj->render(pRender);
        }
    }

    void Container::postRender(core::Renderer *pRender)
    {
        for (const auto &obj : objects)
        {
            if (obj != nullptr)
                obj->postRender(pRender);
        }
    }
    void Container::handleEvents(core::Input *pInput)
    {
        for (const auto &obj : objects)
        {
            if (obj != nullptr)
                obj->handleEvents(pInput);
        }
    }

    size_t Container::size()
    {
        return objects.size();
    }

    std::shared_ptr<Object> Container::get(size_t pos)
    {
        return objects.at(pos);
    }

    void Container::needsRefresh()
    {
        needRefresh = true;
    }

    void Container::refresh()
    {
        endRefresh();
    }

    void Container::endRefresh()
    {
        needRefresh = false;
    }
    void Container::clear()
    {
        objects.clear();
    }

} // namespace UI
