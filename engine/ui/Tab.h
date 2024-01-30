/*
 * Tab.h
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#ifndef UI_TAB_H_
#define UI_TAB_H_

#include <string>
#include "engine/ui/Container.h"

namespace UI
{

    class Tab : public UI::Container, public UI::Object
    {
    public:
        Tab(Object *parent, const std::string &title);
        virtual ~Tab();
        const std::string &getTitle() const;
        void setTitle(const std::string &title);
        void render(core::Renderer *pRender) override;
        bool handleEvents(core::Input *pInput) override;
        void postRender(core::Renderer *renderer) override;

    private:
        std::string title;
    };

} /* namespace UI */

#endif /* UI_TAB_H_ */
