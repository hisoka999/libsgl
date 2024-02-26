#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include <memory>
#include <vector>
#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/ui/Object.h"

namespace UI
{
    namespace layout
    {
        class Layout;
    }
    typedef std::vector<std::shared_ptr<Object>> ObjectList;
    class Container
    {
    public:
        /** Default constructor */
        Container();
        /** Default destructor */
        virtual ~Container();
        void addObject(const std::shared_ptr<Object> &obj);
        void removeObject(const std::shared_ptr<Object> &obj);
        virtual void clear();
        virtual void render(core::Renderer *pRender);
        virtual void postRender(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);
        size_t size();
        std::shared_ptr<Object> get(size_t pos);
        void needsRefresh();
        void setLayout(const std::shared_ptr<layout::Layout> &layout);
        void updateLayout();

    protected:
        ObjectList objects;
        virtual void refresh();
        void endRefresh();
        virtual void boundsRect(graphics::Rect &rect);

    private:
        bool needRefresh = false;
        std::shared_ptr<layout::Layout> m_layout = nullptr;
    };

} // namespace UI

#endif // UI_CONTAINER_H
