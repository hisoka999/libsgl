#include "Theme.h"

#include <engine/ui/Object.h>
#include <magic_enum.hpp>
#include <engine/utils/json/parser.h>
#include <memory>
#include <fstream>
namespace UI
{
    std::shared_ptr<utils::JSON::Object> Theme::findObjectStyle(UI::Object *object) const
    {
        auto styleClass = object->getStyleClass();
        auto elementName = object->getObjectName();

        if (!styleClass.empty() && themeElements->hasAttribute(styleClass))
        {
            auto themeElement = themeElements->getObjectValue(styleClass);
            return themeElement;
        }
        return themeElements->getObjectValue(elementName);
    }

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

    std::string Theme::getStyleText(UI::Object *object, const StyleType styleType) const
    {
        std::string name = std::string(magic_enum::enum_name(styleType));

        return findObjectStyle(object)->getStringValue(name);
    }

    SDL_Color Theme::getStyleColor(UI::Object *object, const StyleType styleType) const
    {
        std::string name = std::string(magic_enum::enum_name(styleType));

        std::string colorString = findObjectStyle(object)->getStringValue(name);
        SDL_Color color;
        auto value = colorString.substr(1); //get rid of the shebang
        color.r = std::stoul(value.substr(0, 2), nullptr, 16);
        color.g = std::stoul(value.substr(2, 2), nullptr, 16);
        color.b = std::stoul(value.substr(4, 2), nullptr, 16);
        color.a = std::stoul(value.substr(6, 2), nullptr, 16);
        return color;
    }

    int Theme::getStyleInt(UI::Object *object, const StyleType styleType) const
    {
        std::string name = std::string(magic_enum::enum_name(styleType));

        return findObjectStyle(object)->getIntValue(name);
    }

} // namespace UI
