#include "engine/ui/Object.h"
#include <engine/graphics/TextureManager.h>
#include <stdexcept>

namespace UI
{

    Object::Object(Object *parent)
        : parent(parent)
    {
        x = y = 0;
        width = height = 0;
        font = nullptr;
        renderOrder = 0;
        showHint = false;
        hint = nullptr;
    }
    Object::Object(Object *parent, int pWidth, int pHeight)
        : renderOrder(0), parent(parent), x(0), y(0), width(pWidth), height(pHeight)
    {

        font = nullptr;
        showHint = false;
        hint = nullptr;
    }

    Object::~Object()
    {
        // dtor
    }
    int Object::getRenderOrder()
    {
        return renderOrder;
    }

    void Object::setHint(const std::shared_ptr<UI::Hint> &hint)
    {
        this->hint = hint;
    }

    const std::shared_ptr<UI::Hint> &Object::getHint()
    {
        return hint;
    }

    std::string &Object::getStyleClass()
    {
        return styleClass;
    }

    void Object::setStyleClass(const std::string &value)
    {
        styleClass = value;
    }

    std::string &Object::getObjectName()
    {
        return objectName;
    }

    const std::shared_ptr<Theme> Object::getTheme()
    {
        if (theme != nullptr)
            return theme;

        if (getParent() != nullptr)
        {
            return getParent()->getTheme();
        }
        return nullptr;
    }

    void Object::setTheme(const std::shared_ptr<Theme> &theme)
    {
        this->theme = theme;
    }

    void Object::setObjectName(const std::string &objectName)
    {
        this->objectName = objectName;
    }

    void Object::render(core::Renderer *pRender)
    {
        if (showHint)
        {
            hint->render(pRender);
        }
    }
    void Object::handleEvents(core::Input *pInput)
    {
        if (hint != nullptr)
        {
            showHint = false;
            if (eventRect().intersects(pInput->getMousePostion()))
            {
                showHint = true;
                hint->setPosition(pInput->getMousePostion());
            }
        }
    }
    graphics::Text *Object::getFont()
    {
        if (font != nullptr)
        {
            return font;
        }
        else if (parent != nullptr)
        {
            return parent->getFont();
        }
        throw std::runtime_error(
            "Object::getFont(): there was no font loaded for this object");
        return nullptr;
    }
    void Object::setFont(graphics::Text *pFont)
    {
        font = pFont;
    }

    void Object::setFont(const std::string &fontname, unsigned int font_size)
    {
        auto text = graphics::TextureManager::Instance().loadFont(fontname, font_size);
        font = text.get();
    }
    graphics::Rect Object::eventRect()
    {
        graphics::Rect r;
        r.x = float(x);
        r.y = float(y);
        r.width = float(width);
        r.height = float(height);
        if (getParent() != nullptr)
        {
            r.x += getParent()->eventRect().x;
            r.y += getParent()->eventRect().y;
        }
        return r;
    }

    graphics::Rect Object::displayRect()
    {
        graphics::Rect r;
        r.x = float(x);
        r.y = float(y);
        r.width = float(width);
        r.height = float(height);
        if (getParent() != nullptr)
        {
            r.x += getParent()->displayRect().x;
            r.y += getParent()->displayRect().y;
        }
        return r;
    }

    bool Object::isShowHint()
    {
        return showHint;
    }
} // namespace UI
