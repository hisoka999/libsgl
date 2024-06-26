/*
 * Tab.cpp
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#include "engine/ui/Tab.h"

namespace UI
{

    Tab::Tab(Object *parent, const std::string &title) : UI::Container(), UI::Object(parent), title(title)
    {
        setStyleClass("Tab");
        setObjectName("Tab");
        if (parent)
        {
            setWidth(parent->getWidth());
            setHeight(parent->getHeight());
        }
    }
    const std::string &Tab::getTitle() const { return title; }
    void Tab::setTitle(const std::string &title) { this->title = title; }
    Tab::~Tab()
    {
        // TODO Auto-generated destructor stub
    }
    void Tab::render(core::Renderer *pRender) { UI::Container::render(pRender); }
    bool Tab::handleEvents(core::Input *pInput) { return UI::Container::handleEvents(pInput); }

    void Tab::postRender(core::Renderer *renderer) { UI::Container::postRender(renderer); }

} /* namespace UI */
