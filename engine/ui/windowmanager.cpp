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
        containers.push_back(win);
    }

    void WindowManager::render(core::Renderer *ren)
    {
        for (auto c : containers)
        {
            c->render(ren);
        }
    }

    void WindowManager::handleInput(core::Input *inp)
    {

        for (auto c : containers)
        {
            Window *win = dynamic_cast<Window *>(c);
            if (win == nullptr)
                c->handleEvents(inp);
        };
        auto win = lastActiveWindow();
        if (win != nullptr)
        {
            win->handleEvents(inp);
        }
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
            if (win->getVisible())
                result = win;
        }
        return result;
    }

} // namespace UI
