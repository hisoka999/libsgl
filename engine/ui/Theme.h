#ifndef __THEME_H__
#define __THEME_H__

#include <string>
#include "engine/utils/json/object.h"
#include <SDL2/SDL.h>

namespace UI
{
    class Object;

    enum class StyleType
    {
        ForgroundColor,
        BackgroundColor,
        BackgroundImage,
        BorderColor,
        TitleColor,
        Color,
        HoverColor,
        DisabledColor,
        FontName,
        FontSize,
        IconFontName,
        IconFontSize,

    };

    class Theme
    {
    private:
        std::shared_ptr<utils::JSON::Object> themeElements;

        std::shared_ptr<utils::JSON::Object> findObjectStyle(UI::Object *object) const;
        std::shared_ptr<utils::JSON::Object> findObjectStyle(const std::string styleClass, const std::string &elementName) const;

        SDL_Color colorStringToColor(std::string colorString) const;

    public:
        Theme(std::string themeName);
        ~Theme();

        std::string getStyleText(UI::Object *object, const StyleType styleType) const;
        SDL_Color getStyleColor(UI::Object *object, const StyleType StyleType) const;
        SDL_Color getStyleColor(const std::string &elementName, const StyleType StyleType) const;
        int getStyleInt(UI::Object *object, const StyleType StyleType) const;
    };

} // namespace UI

#endif // __THEME_H__