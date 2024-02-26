#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <iostream>
#include <map>

#include <memory>
#include "engine/core/dispatcher.h"
#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/graphics/rect.h"
#include "engine/graphics/text.h"
#include "engine/graphics/texture.h"
#include "engine/ui/Hint.h"

namespace UI
{
    class Theme;

    class BaseObject
    {
    };

    typedef std::function<bool(UI::Object *, std::string &)> CheckDropCallBack;

    class Object : public BaseObject
    {

    public:
        /** Default constructor */
        explicit Object(Object *parent = nullptr);
        Object(Object *parent, int pWidth, int pHeight);
        /** Default destructor */
        virtual ~Object();
        void setX(int x);
        void setY(int y);
        void setPos(int x, int y);
        [[nodiscard]] int getX() const;
        [[nodiscard]] int getY() const;
        virtual int getWidth() { return width; }
        virtual int getHeight() { return height; }
        void setWidth(int pWidth) { width = pWidth; }
        void setHeight(int pHeight) { height = pHeight; }

        virtual void render(core::Renderer *renderer);
        virtual void postRender(core::Renderer *renderer);
        virtual bool handleEvents(core::Input *pInput);
        [[nodiscard]] Object *getParent() const { return parent; }
        void connect(std::string const &event, core::dispatcher_type callback) { _callbacks.emplace(event, callback); }

        template<typename F>
        void connect(std::string const &event, F &&f)
        {
            _callbacks.emplace(event, core::make_dispatcher(std::forward<F>(f)));
        }

        template<typename F>
        void disconnect(std::string const &event, F &&f)
        {
            auto rng = _callbacks.equal_range(event);
            for (auto it = rng.first; it != rng.second; ++it)
            {
                if (it->second == f)
                {
                    _callbacks.erase(it);
                }
            }
        }
        void disconnect(std::string const &event)
        {
            auto rng = _callbacks.equal_range(event);
            for (auto it = rng.first; it != rng.second; ++it)
            {
                _callbacks.erase(it);
            }
        }

        graphics::Text *getFont() const;
        void setFont(graphics::Text *pFont);
        void setFont(const std::string &fontname, unsigned int font_size);

        virtual graphics::Rect displayRect();
        virtual graphics::Rect eventRect();
        int getRenderOrder() const;
        void setHint(const std::shared_ptr<UI::Hint> &hint);
        const std::shared_ptr<UI::Hint> &getHint();
        std::string &getStyleClass();
        void setStyleClass(const std::string &value);
        std::string &getObjectName();

        std::shared_ptr<Theme> getTheme();
        void setTheme(const std::shared_ptr<Theme> &theme);
        void setCheckDropCallBack(const CheckDropCallBack &callback);
        CheckDropCallBack getCheckDropCallBack();
        [[nodiscard]] bool isVisible();
        virtual void setVisible(bool visible);

    protected:
        void setObjectName(const std::string &objectName);
        template<typename... Args>
        bool fireFuncionCall(std::string const &event, Args const &...args)
        {
            bool functionCalled = false;
            auto rng = _callbacks.equal_range(event);
            for (auto it = rng.first; it != rng.second; ++it)
            {
                call(it->second, args...);
                functionCalled = true;
            }
            return functionCalled;
        }
        int renderOrder;
        bool isShowHint() const;

    private:
        std::shared_ptr<UI::Hint> hint;
        bool showHint;

        template<typename F, typename... Args>
        static void call(F const &f, Args const &...args)
        {
            std::vector<std::any> v{args...};
            f(v);
        }

        Object *parent;
        int x, y;
        int width, height;

        std::multimap<std::string, core::dispatcher_type> _callbacks;
        graphics::Text *font;
        std::string styleClass;
        std::string objectName;
        std::shared_ptr<Theme> theme = nullptr;
        CheckDropCallBack dragCallback = nullptr;
        bool visible = false;
    };

} // namespace UI

#endif // UI_OBJECT_H
