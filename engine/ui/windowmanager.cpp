#include "engine/ui/windowmanager.h"
#include "engine/graphics/TextureManager.h"
#include "engine/ui/Window.h"
#include "engine/utils/os.h"
#include <algorithm>

namespace UI
{

    WindowManager::WindowManager()

    {
        addWindow(&messageDialog);

        font = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 12);
    }

    WindowManager::~WindowManager()
    {
        containers.clear();
        font = nullptr;
    }

    void WindowManager::addContainer(Container *c)
    {
        containers.push_back(c);
    }
    void WindowManager::addWindow(Window *win)
    {
        win->setFont(font.get());
        win->connect("windowActiveChanged", [this](Window *changedWin)
                     {
            if(!changedWin->isActive())
                return;
            for(auto& container : containers) {
                Window *_win = dynamic_cast<Window *>(container);
                if (_win == nullptr)
                    continue;

                if(_win != changedWin) {
                    _win->makeInactive();
                }
            } });
        containers.push_back(win);
    }

    void WindowManager::render(core::Renderer *ren)
    {
        for (auto c : containers)
        {
            c->render(ren);
            c->postRender(ren);
        }

        if (dragActive)
        {
            // render drag preview element
        }
    }

    bool WindowManager::handleInput(core::Input *input)
    {
        bool eventHandled = false;
        dragActive = input->isDragActive();
        for (auto c : containers)
        {
            Window *win = dynamic_cast<Window *>(c);
            if (win == nullptr)
                eventHandled = c->handleEvents(input);
            else
                eventHandled = win->handleWindowEvents(input);

            if (eventHandled)
                break;
        };
        if (!eventHandled)
        {
            auto win = lastActiveWindow();
            if (win != nullptr)
            {
                eventHandled = win->handleEvents(input);
            }
        }
        return eventHandled;
    }

    bool WindowManager::isWindowOpen()
    {
        for (auto container : containers)
        {
            Window *win = dynamic_cast<Window *>(container);
            if (win == nullptr)
                continue;
            if (win->getVisible())
                return true;
        }
        return false;
    }

    Window *WindowManager::lastActiveWindow()
    {
        Window *result = nullptr;
        for (auto container : containers)
        {
            Window *win = dynamic_cast<Window *>(container);
            if (win == nullptr)
                continue;
            if (win->getVisible() && win->isActive())
                result = win;
        }
        return result;
    }

} // namespace UI
