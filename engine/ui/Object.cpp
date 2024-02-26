#include "engine/ui/Object.h"
#include <engine/graphics/TextureManager.h>
#include <stdexcept>

namespace UI
{

    Object::Object(Object *parent) : parent(parent)
    {
        x = y = 0;
        width = height = 0;
        font = nullptr;
        renderOrder = 0;
        showHint = false;
        hint = nullptr;
    }
    Object::Object(Object *parent, int pWidth, int pHeight) :
        renderOrder(0), parent(parent), x(0), y(0), width(pWidth), height(pHeight)
    {

        font = nullptr;
        showHint = false;
        hint = nullptr;
    }

    Object::~Object()
    {
        // dtor
    }
    int Object::getRenderOrder() const { return renderOrder; }

    void Object::setHint(const std::shared_ptr<UI::Hint> &hint) { this->hint = hint; }

    const std::shared_ptr<UI::Hint> &Object::getHint() { return hint; }

    std::string &Object::getStyleClass() { return styleClass; }

    void Object::setStyleClass(const std::string &value) { styleClass = value; }

    std::string &Object::getObjectName() { return objectName; }

    std::shared_ptr<Theme> Object::getTheme()
    {
        if (theme != nullptr)
            return theme;

        if (getParent() != nullptr)
        {
            return getParent()->getTheme();
        }
        return nullptr;
    }

    void Object::setTheme(const std::shared_ptr<Theme> &theme) { this->theme = theme; }

    void Object::setObjectName(const std::string &objectName) { this->objectName = objectName; }

    void Object::render([[maybe_unused]] core::Renderer *renderer) {}

    void Object::postRender(core::Renderer *renderer)
    {
        if (showHint)
        {
            hint->render(renderer);
        }
    }

    bool Object::handleEvents(core::Input *pInput)
    {
        if (hint != nullptr)
        {
            showHint = false;
            if (eventRect().intersects(pInput->getMousePostion()))
            {
                showHint = true;
                hint->setPosition(pInput->getMousePostion() + utils::Vector2(16, 16));
            }
        }
        return false;
    }
    graphics::Text *Object::getFont() const
    {
        if (font != nullptr)
        {
            return font;
        }
        if (parent != nullptr)
        {
            return parent->getFont();
        }
        throw std::runtime_error("Object::getFont(): there was no font loaded for this object");
        return nullptr;
    }
    void Object::setFont(graphics::Text *pFont) { font = pFont; }

    void Object::setFont(const std::string &fontname, unsigned int font_size)
    {
        const auto text = graphics::TextureManager::Instance().loadFont(fontname, font_size);
        font = text.get();
    }
    graphics::Rect Object::eventRect()
    {
        graphics::Rect r;
        r.x = static_cast<float>(x);
        r.y = static_cast<float>(y);
        r.width = static_cast<float>(width);
        r.height = static_cast<float>(height);
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
        r.x = static_cast<float>(x);
        r.y = static_cast<float>(y);
        r.width = static_cast<float>(width);
        r.height = static_cast<float>(height);
        if (getParent() != nullptr)
        {
            r.x += getParent()->displayRect().x;
            r.y += getParent()->displayRect().y;
        }
        return r;
    }

    bool Object::isShowHint() const { return showHint; }

    void Object::setCheckDropCallBack(const CheckDropCallBack &callback) { dragCallback = callback; }

    CheckDropCallBack Object::getCheckDropCallBack() { return dragCallback; }
    void Object::setX(int x) { this->x = x; }
    void Object::setY(int y) { this->y = y; }
    void Object::setPos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    int Object::getX() const { return x; }
    int Object::getY() const { return y; }

    bool Object::isVisible() { return visible; }
    void Object::setVisible(bool visible) { this->visible = visible; }

} // namespace UI
