#include "engine/core/input.h"
#include "engine/ui/Object.h"

namespace core
{

    Input::Input()
        : mousePosition(-1, -1)
    {
        // ctor
    }

    Input::Input(const KeyMap &keyMap)
        : mousePosition(-1, -1), keyMap(keyMap)
    {
    }

    bool Input::poll()
    {
        bool retval = SDL_PollEvent(&event);
        if (retval)
        {
            if (event.type == SDL_KEYDOWN && event.key.state == SDL_PRESSED)
            {
                pressedKeys[event.key.keysym.sym] = true;
            }
            else if (event.type == SDL_KEYUP && event.key.state == SDL_RELEASED)
            {
                pressedKeys[event.key.keysym.sym] = false;
            }
            if (isMouseMoving())
            {
                mousePosition = utils::Vector2(float(getEvent().motion.x), float(getEvent().motion.y));
            }

            if (isDragActive() && isMouseButtonUp(SDL_BUTTON_LEFT))
            {
                dragContext.active = false;
                if (dragContext.dragCallBack)
                {
                    dragContext.dragCallBack(dragContext.startObject, dragContext.target, dragContext.data, dragContext.dropFailed);
                    dragContext.checkDropCallback = nullptr;
                    dragContext.data = "";
                    dragContext.startObject = nullptr;
                    dragContext.dragCallBack = nullptr;
                    dragContext.dropFailed = false;
                }
            }
        }
        return retval;
    }

    bool Input::isQuit()
    {
        return (event.type == SDL_QUIT);
    }

    bool Input::isKeyDown(SDL_Keycode key)
    {
        return pressedKeys.count(key) > 0 && pressedKeys[key];
        if (event.type == SDL_KEYDOWN && key == event.key.keysym.sym && event.key.state == SDL_PRESSED)
            return true;
        return false;
    }
    bool Input::isKeyUp(SDL_Keycode key)
    {
        if (event.type == SDL_KEYUP && key == event.key.keysym.sym && event.key.state == SDL_RELEASED)
            return true;
        return false;
    }

    bool Input::isKeyDown(const std::string &key)
    {
        auto result = keyMap.equal_range(key);
        for (auto it = result.first; it != result.second; it++)
        {
            if (isKeyDown(it->second))
                return true;
        }
        return false;
    }

    void Input::keyDown(bool *keyDown, SDL_Keycode *keyCode)
    {
        *keyDown = event.type == SDL_KEYDOWN;
        if (*keyDown)
        {
            *keyCode = event.key.keysym.sym;
        }
        else
        {
            keyCode = nullptr;
        }
    }

    bool Input::isKeyUp(const std::string &key)
    {
        auto result = keyMap.equal_range(key);
        for (auto it = result.first; it != result.second; it++)
        {
            if (isKeyUp(it->second))
                return true;
        }
        return false;
    }

    bool Input::isMouseButtonPressed(int button)
    {
        return (getEvent().button.button == button && event.type == SDL_MOUSEBUTTONDOWN);
    }
    bool Input::isMouseButtonUp(int button)
    {
        return (getEvent().button.button == button && event.type == SDL_MOUSEBUTTONUP);
    }

    bool Input::isMouseMoving()
    {
        return (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN);
    }
    bool Input::isMouseDoubleClick()
    {
        return isMouseButtonPressed(SDL_BUTTON_LEFT) && getEvent().button.clicks >= 2;
    }
    bool Input::isScrollWheel()
    {
        return event.type == SDL_MOUSEWHEEL;
    }

    utils::Vector2 Input::getMouseWheelPosition()
    {
        return utils::Vector2(float(event.wheel.x), float(event.wheel.y));
    }

    utils::Vector2 &Input::getMousePostion()
    {

        return mousePosition;
    }

    Input::~Input()
    {
        // dtor
    }

    std::string Input::getTextInput()
    {
        return std::string(event.edit.text);
    }
    bool Input::isTextInput()
    {
        return isTextInputActive() && event.type == SDL_TEXTINPUT;
    }

    void Input::startTextInput(const graphics::Rect &rect)
    {
        SDL_Rect r = rect.sdlRect();
        SDL_SetTextInputRect(&r);
        SDL_StartTextInput();
    }

    void Input::stopTextInput()
    {
        SDL_StopTextInput();
    }

    bool Input::isTextInputActive()
    {
        return SDL_IsTextInputActive();
    }

    KeyMap Input::getKeyMap()
    {
        return keyMap;
    }

    bool Input::isDragActive()
    {
        return dragContext.active;
    }

    bool Input::canDropOnTarget(UI::Object *target)
    {
        if (!target || !target->eventRect().intersects(getMousePostion()) || target == dragContext.startObject)
        {
            return false;
        }
        if (!dragContext.checkDropCallback)
            dragContext.dropFailed = true;
        else
            dragContext.dropFailed = !dragContext.checkDropCallback(target);

        dragContext.target = target;

        return !dragContext.dropFailed;
    }

    void Input::beginDrag(const std::string &data, UI::Object *source, DragCallBack dragCallBack, CheckDropCallBack checkDropCallback)
    {
        dragContext.active = true;
        dragContext.data = data;
        dragContext.target = nullptr;
        dragContext.startObject = source;
        dragContext.dragCallBack = dragCallBack;
        dragContext.checkDropCallback = checkDropCallback;
    }

} // namespace core
