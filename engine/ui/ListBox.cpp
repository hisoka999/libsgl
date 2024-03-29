#include "engine/ui/ListBox.h"
#include "engine/graphics/gameobject.h"
#include "engine/graphics/TextureManager.h"
namespace UI
{

    ListBox::ListBox(Object *parent)
        : UI::Object(parent)
    {
        font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
    }

    ListBox::~ListBox()
    {
    }

    ListBoxSelections ListBox::getSelections()
    {
        ListBoxSelections selections;
        for (unsigned int i = 0; i < items.size(); ++i)
            if (items[i].selected)
                selections.push_back(i);
        return selections;
    }
    void ListBox::addItem(std::string item)
    {
        ListBoxItem it;
        it.text = item;
        it.selected = false;
        items.push_back(it);
    }
    void ListBox::removeItem(int pos)
    {
        items.erase(items.begin() + pos);
    }

    bool ListBox::handleEvents(core::Input *pInput)
    {

        graphics::Rect rect;
        if (getParent() != nullptr)
        {
            rect.x = getParent()->getX() + getX();
            rect.y = getParent()->getY() + getY();
        }
        else
        {
            rect.x = getX();
            rect.y = getY();
        }
        rect.width = 23;
        rect.height = items.size() * rect.width;
        utils::Vector2 pos = pInput->getMousePostion();
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT) && rect.intersects(pos))
        {
            // fint the selected item
            int res = (pos.getY() - rect.y) / 23;
            items[res].selected = !items[res].selected;
            return true;
        }
        return false;
    }
    void ListBox::render(core::Renderer *pRender)
    {
        int tx = this->getParent()->getX() + getX();
        int ty = this->getParent()->getY() + getY();
        for (unsigned int i = 0; i < items.size(); ++i)
        {
            ListBoxItem item = items[i];
            int tx2 = 0;
            if (item.selected)
            {
                tx2 += 23;
            }
            SDL_Color color = {255, 255, 255, 255};
            font->render(pRender, item.text, color, tx + 40, ty + i * 30);
        }
    }
} // namespace UI
