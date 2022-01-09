#pragma once
#ifndef UI_TABLE_H
#define UI_TABLE_h
#include "Object.h"
#include <vector>

namespace UI
{
    template <typename T>
    class Table : public Object
    {

    public:
        Table(UI::Object *parent = nullptr);
        ~Table();

        virtual void render(core::Renderer *renderer);
        // virtual void postRender(core::Renderer *renderer);
        virtual void handleEvents(core::Input *input);
        void setData(std::vector<std::shared_ptr<T>> &data)
        {
            m_data = data;
        }

        void setHeaderNames(const std::vector<std::string> &headerNames);

        void setElementFunction(size_t col, std::function<std::string(std::shared_ptr<T>)> displayFunction)
        {
            m_cellRenderer[col] = displayFunction;
        }

    private:
        /* data */
        std::vector<std::shared_ptr<T>> m_data;
        std::vector<std::string> m_headerNames;
        std::vector<std::function<std::string(std::shared_ptr<T>)>> m_cellRenderer;

        SDL_Color m_borderColor;
        SDL_Color m_headerColor;
    };

} // namespace UI

#endif