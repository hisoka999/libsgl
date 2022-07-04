#include "Theme.h"

#include <engine/ui/Object.h>
#include <magic_enum.hpp>
#include <engine/utils/json/parser.h>
#include <memory>
#include <fstream>
namespace UI
{
    Theme::Theme(std::string themeName)
    {

        std::ifstream file;
        std::istringstream is;
        std::string s;
        std::string group;
        //  std::cout << filename << std::endl;

        file.open(themeName.c_str());
        assert(file.is_open());

        std::string buffer((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());

        utils::JSON::Parser parser;
        themeElements = parser.parseObject(buffer);

        file.close();
    }

    Theme::~Theme()
    {
    }

    std::shared_ptr<utils::JSON::Object> Theme::findObjectStyle(UI::Object *object) const
    {
        auto styleClass = object->getStyleClass();
        auto elementName = object->getObjectName();
        return findObjectStyle(styleClass, elementName);
    }

    std::shared_ptr<utils::JSON::Object> Theme::findObjectStyle(const std::string styleClass, const std::string &elementName) const
    {
        if (!styleClass.empty() && themeElements->hasAttribute(styleClass))
        {
            auto themeElement = themeElements->getObjectValue(styleClass);
            return themeElement;
        }
        return themeElements->getObjectValue(elementName);
    }

    SDL_Color Theme::colorStringToColor(std::string colorString) const
    {
        SDL_Color color;
        auto value = colorString.substr(1); // get rid of the shebang
        color.r = std::stoul(value.substr(0, 2), nullptr, 16);
        color.g = std::stoul(value.substr(2, 2), nullptr, 16);
        color.b = std::stoul(value.substr(4, 2), nullptr, 16);
        color.a = std::stoul(value.substr(6, 2), nullptr, 16);
        return color;
    }

    std::string Theme::getStyleText(UI::Object *object, const StyleType styleType) const
    {
        std::string name = std::string(magic_enum::enum_name(styleType));

        return findObjectStyle(object)->getStringValue(name);
    }

    SDL_Color Theme::getStyleColor(UI::Object *object, const StyleType styleType) const
    {
        std::string name = std::string(magic_enum::enum_name(styleType));

        std::string colorString = findObjectStyle(object)->getStringValue(name);
        return colorStringToColor(colorString);
    }

    SDL_Color Theme::getStyleColor(const std::string &elementName, const StyleType StyleType) const
    {
        std::string name = std::string(magic_enum::enum_name(StyleType));

        std::string colorString = findObjectStyle("", elementName)->getStringValue(name);
        return colorStringToColor(colorString);
    }

    int Theme::getStyleInt(UI::Object *object, const StyleType styleType) const
    {
        std::string name = std::string(magic_enum::enum_name(styleType));

        return findObjectStyle(object)->getIntValue(name);
    }

} // namespace UI
