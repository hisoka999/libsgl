#pragma once

#include "Object.h"

namespace UI
{

    class VerticalScrollbar : public UI::Object
    {
    private:
        SDL_Color m_backgroundColor;
        SDL_Color m_foregroundColor;
        std::shared_ptr<graphics::Text> uiText;

        int m_scrollPosition = 0;
        int m_contentHeight = 0;


    public:
        VerticalScrollbar(UI::Object *parent);
        ~VerticalScrollbar() = default;
        void render(core::Renderer *renderer) override;
        void postRender(core::Renderer *renderer) override;
        bool handleEvents(core::Input *pInput) override;
        void setContentHeight(int contentHeight);
        int getScrollPosition();
    };
} // namespace UI
