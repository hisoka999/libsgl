#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

#include <SDL2/SDL.h>
#include <engine/utils/vector2.h>
namespace graphics
{

    struct Rect
    {
        float x;
        float y;
        float width;
        float height;

        Rect()
        {
            x = 0;
            y = 0;
            width = 0;
            height = 0;
        }
        Rect(float pX, float pY, float pWidth, float pHeight)
        {
            x = pX;
            y = pY;
            width = pWidth;
            height = pHeight;
        }
        bool intersectsNoLine(const Rect &target) const
        {
            if (
                (x + width > target.x) &&
                (y + height > target.y) &&
                (x < target.x + target.width) &&
                (y < target.y + target.height))
                return true;
            else
                return false;
        }
        bool intersects(const Rect &target) const
        {
            if (
                (x + width >= target.x) &&
                (y + height >= target.y) &&
                (x <= target.x + target.width) &&
                (y <= target.y + target.height))
                return true;
            else
                return false;
        }
        bool intersects(const utils::Vector2 &v) const
        {
            if (x <= v.getX() && y <= v.getY() && x + width >= v.getX() && y + height >= v.getY())
                return true;
            return false;
        }

        const SDL_Rect sdlRect() const
        {
            SDL_Rect r;
            r.h = int(height);
            r.w = int(width);
            r.x = int(x);
            r.y = int(y);
            return r;
        }

        const SDL_FRect sdlFRect() const
        {
            SDL_FRect r;
            r.h = height;
            r.w = width;
            r.x = x;
            r.y = y;
            return r;
        }
        bool operator==(const Rect &other) const
        {
            return other.x == x && other.y == y && other.width == width && other.height == height;
        }
        bool operator!=(const Rect &other) const
        {
            return other.x != x || other.y != y || other.width != width || other.height != height;
        }
    };

}

#endif // RECT_H_INCLUDED
