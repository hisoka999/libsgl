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

        void setElementFunction(size_t col, std::function<std::string(std::shared_ptr<T> &)> displayFunction)
        {
            m_cellRenderer.resize(m_headerNames.size());
            m_cellRenderer[col] = displayFunction;
        }

    private:
        /* data */
        std::vector<std::shared_ptr<T>> m_data;
        std::vector<std::string> m_headerNames;
        std::vector<std::function<std::string(std::shared_ptr<T> &)>> m_cellRenderer;

        SDL_Color m_borderColor;
        SDL_Color m_headerColor;
        SDL_Color m_textColor;
    };

} // namespace UI
#include "engine/graphics/TextureManager.h"

namespace UI
{
    template <typename T>
    Table<T>::Table(UI::Object *parent)
        : UI::Object(parent)
    {
        setObjectName("table");
        if (parent == nullptr || getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }

        m_borderColor = getTheme()->getStyleColor(this, StyleType::BorderColor);
        m_headerColor = getTheme()->getStyleColor(this, StyleType::TitleColor);
        m_textColor = getTheme()->getStyleColor(this, StyleType::Color);
    }

    template <typename T>
    Table<T>::~Table()
    {
    }

    template <typename T>
    void Table<T>::render(core::Renderer *renderer)
    {

        // render header
        renderer->setDrawColor(m_borderColor);
        renderer->drawRect(displayRect());
        //
        int x = displayRect().x + 10;

        int yHeader = displayRect().y;
        const float rowHeight = 25.f;
        for (std::string &header : m_headerNames)
        {

            getFont()->render(renderer, header, m_headerColor, x, yHeader + 5);
            x += 100; // for now fixed width
        }
        yHeader += rowHeight;
        for (size_t row = 0; row < m_data.size(); ++row)
        {
            auto &value = m_data[row];
            int y = yHeader + (float(row) * rowHeight);

            for (size_t col = 0; col < m_cellRenderer.size(); ++col)
            {
                int x = displayRect().x + (col * 100) + 10;

                std::string cellValue = m_cellRenderer[col](value);
                getFont()->render(renderer, cellValue, m_textColor, x, y + 5);
            }
            utils::Vector2 lineStart(displayRect().x, yHeader + (float(row) * rowHeight));
            utils::Vector2 lineEnd(displayRect().x + displayRect().width, yHeader + (float(row) * rowHeight));
            renderer->drawLine(lineStart, lineEnd);
        }
    }

    template <typename T>
    void Table<T>::handleEvents(core::Input *input)
    {

        UI::Object::handleEvents(input);
    }

    template <typename T>
    void Table<T>::setHeaderNames(const std::vector<std::string> &headerNames)
    {
        m_headerNames = headerNames;
    }

}

#endif