#include "Table.h"
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
        for (std::string &header : m_headerNames)
        {

            getFont()->render(renderer, header, m_headerColor, x, displayRect().y + 5);
            x += 100; // for now fixed width
        }

        for (size_t row = 0; row <= m_data.size(); ++row)
        {
            auto &value = m_data[row];
            int y = displayRect().y + (float(row) * 20.f);

            for (size_t col = 0; col <= m_cellRenderer.size(); ++col)
            {
                int x = displayRect().x + (col * 100);

                std::string cellValue = m_cellRenderer[col](value);
                getFont()->render(renderer, cellValue, m_headerColor, x, y);
            }
            utils::Vector2 lineStart(displayRect().x, displayRect().y + (float(row) * 20.f));
            utils::Vector2 lineEnd(displayRect().x + displayRect().width, displayRect().y + (float(row) * 20.f));
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