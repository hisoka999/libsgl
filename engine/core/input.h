#ifndef CORE_INPUT_H
#define CORE_INPUT_H

#include "engine/utils/vector2.h"
#include <SDL2/SDL.h>
#include <map>
#include <string>
#include "engine/graphics/rect.h"
#include <functional>

namespace UI
{
    class Object;
}

namespace core
{
    typedef std::function<void(UI::Object *, UI::Object *, const std::string &, bool)> DragCallBack;
    typedef std::function<bool(UI::Object *, std::string &)> CheckDropCallBack;
    struct DragContext
    {
        bool active = false;
        bool dropFailed = false;
        UI::Object *startObject = nullptr;
        UI::Object *target = nullptr;
        std::string data;
        DragCallBack dragCallBack = nullptr;
    };

    typedef std::multimap<std::string, SDL_Keycode> KeyMap;

    class Input
    {
    public:
        /** Default constructor */
        Input();
        Input(const KeyMap &keyMap);
        /** Default destructor */
        virtual ~Input();
        bool isKeyDown(SDL_Keycode key);
        bool isKeyUp(SDL_Keycode key);
        bool isKeyDown(const std::string &key);
        void keyDown(bool *keyDown, SDL_Keycode *keyCode);
        bool isKeyUp(const std::string &key);
        bool isMouseButtonPressed(int button);
        bool isMouseButtonUp(int button);
        bool isMouseMoving();
        bool isMouseDoubleClick();
        bool isQuit();
        utils::Vector2 &getMousePostion();
        bool poll();
        // void clearEvents();
        SDL_Event &getEvent()
        {
            return event;
        }
        utils::Vector2 getMouseWheelPosition();
        bool isScrollWheel();
        std::string getTextInput();
        bool isTextInput();

        void startTextInput(const graphics::Rect &rect);
        void stopTextInput();
        bool isTextInputActive();
        KeyMap getKeyMap();
        void beginDrag(const std::string &data, UI::Object *source, DragCallBack dragCallBack = nullptr);
        bool canDropOnTarget(UI::Object *target);
        bool isDragActive();

    protected:
    private:
        SDL_Event event;
        DragContext dragContext;

        std::map<SDL_Keycode, bool> pressedKeys;
        utils::Vector2 mousePosition;
        KeyMap keyMap;
    };

} // namespace core

#endif // CORE_INPUT_H
