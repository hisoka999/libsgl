#include "VerticalScrollbar.h"
#include "engine/graphics/TextureManager.h"
namespace UI
{
    const int BUTTON_HEIGHT = 14;

    VerticalScrollbar::VerticalScrollbar(UI::Object *parent = nullptr) : UI::Object(parent)
    {
        setObjectName("verticalscrollbar");

        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }
        m_backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
        m_foregroundColor = getTheme()->getStyleColor(this, UI::StyleType::ForgroundColor);
        uiText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 12);

        setWidth(14);
    }

    bool VerticalScrollbar::handleEvents(core::Input *pInput)
    {
        float relativeHeight = (getHeight() - ((BUTTON_HEIGHT + 2) * 2)) / static_cast<float>(m_contentHeight);
        float scrollBarHeight = (relativeHeight * (getHeight() - BUTTON_HEIGHT + 2));

        if (pInput->isMouseButtonUp(SDL_BUTTON_LEFT))
        {

            graphics::Rect parentRect;
            if (getParent() != nullptr)
                parentRect = getParent()->displayRect();

            graphics::Rect destRect;

            destRect.width = getWidth();
            destRect.height = getWidth();
            destRect.x = parentRect.x + getX();
            destRect.y = parentRect.y + getY();

            if (destRect.intersects(pInput->getMousePostion()) && m_scrollPosition > 5)
            {
                m_scrollPosition -= 5;
                SGL_LOG_TRACE(" clicked on top button");
            }

            destRect.y = parentRect.y + getHeight() + getY() - BUTTON_HEIGHT;

            // render right edge
            destRect.x = parentRect.x + getX();
            if (destRect.intersects(pInput->getMousePostion()) &&
                m_scrollPosition + scrollBarHeight < getHeight() - (BUTTON_HEIGHT * 2))
            {
                m_scrollPosition += 5;
                SGL_LOG_TRACE(" clicked on bottom button");
            }

            // render bar
            destRect.width = getWidth();
            destRect.height = 5;
            destRect.x = parentRect.x + BUTTON_HEIGHT + getX();
            destRect.y = parentRect.y + getHeight() + 4 + getY();
            if (destRect.intersects(pInput->getMousePostion()))
            {
                SGL_LOG_TRACE(" bar clicked");
            }
        }

        graphics::Rect r = displayRect();

        if (pInput->isScrollWheel())
        {

            utils::Vector2 pos = pInput->getMouseWheelPosition();

            if ((m_scrollPosition >= 0 && pos.getY() == 1) ||
                (pos.getY() == -1 && (relativeHeight * m_scrollPosition) + scrollBarHeight < r.height - (BUTTON_HEIGHT * 2)))
            {
                m_scrollPosition += pos.getY() * -5;
                return true;
            }
        }
        return false;
    }

    void VerticalScrollbar::postRender([[maybe_unused]] core::Renderer *renderer) {}

    void VerticalScrollbar::render(core::Renderer *renderer)
    {
        auto rect = displayRect();
        renderer->drawRect(rect);

        SDL_Color uiColor = m_foregroundColor;
        uiText->render(renderer, "\uf0d7", uiColor, rect.x + 2, rect.y + rect.height - BUTTON_HEIGHT);

        uiText->render(renderer, "\uf0d8", uiColor, rect.x + 2, rect.y + 2);

        rect.x += 2;
        float areaHeight = rect.height;
        float relativeHeight = (getHeight() - ((BUTTON_HEIGHT + 2) * 2)) / static_cast<float>(m_contentHeight);
        rect.y += (relativeHeight * m_scrollPosition) + BUTTON_HEIGHT + 2;
        rect.height = (relativeHeight * (areaHeight - BUTTON_HEIGHT + 2));
        rect.width -= 4;
        renderer->fillRect(rect);
    }

    void VerticalScrollbar::setContentHeight(int contentHeight) { m_contentHeight = contentHeight; }
    int VerticalScrollbar::getScrollPosition() { return m_scrollPosition; }
} // namespace UI
