/*
 * TabBar.h
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#ifndef UI_TABBAR_H_
#define UI_TABBAR_H_

#include "engine/ui/Object.h"
#include "engine/ui/Tab.h"
#include <memory>
#include <vector>

namespace UI
{

    enum class TabDirection
    {
        Top,
        Left,
    };
    class TabBar : public UI::Object
    {
    public:
        TabBar(Object *parent = nullptr, TabDirection tabDirection = TabDirection::Left);
        virtual ~TabBar();

        void addTab(std::shared_ptr<Tab> tab);
        void removeTabByIndex(int index);
        void removeTab(std::shared_ptr<Tab> tab);

        virtual void render(core::Renderer *renderer) override;
        virtual void postRender(core::Renderer *renderer) override;
        virtual bool handleEvents(core::Input *pInput) override;
        virtual graphics::Rect displayRect();
        virtual graphics::Rect eventRect();
        void setCurrentTab(std::shared_ptr<Tab> tab);

    protected:
        const int tabWidth = 120;
        const int tabHeight = 20;
        void renderTop(core::Renderer *renderer);
        void renderLeft(core::Renderer *renderer);
        void calulateTabRect(size_t index, graphics::Rect &rect);

    private:
        std::vector<std::shared_ptr<Tab>> tabs;
        int currentTab;
        int hoveredTab;
        std::shared_ptr<graphics::Texture> btnTexture;
        SDL_Color hoverColor;
        SDL_Color disabledColor;
        SDL_Color color;
        SDL_Color backgroundColor;
        TabDirection tabDirection;
    };

} /* namespace UI */

#endif /* UI_TABBAR_H_ */
