#include "engine/core/renderer.h"
#include <exception>
#include <iostream>
#include <vector>
#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/logger.h"
namespace core
{

    std::vector<SDL_Point> points;
    void sdlRectP(const graphics::Rect &src, SDL_Rect *r)
    {
        r->h = static_cast<int>(src.height);
        r->w = static_cast<int>(src.width);
        r->x = static_cast<int>(src.x);
        r->y = static_cast<int>(src.y);
    }

    void sdlFRectP(const graphics::Rect &src, SDL_FRect *r)
    {
        r->h = src.height;
        r->w = src.width;
        r->x = src.x;
        r->y = src.y;
    }
    inline int vline(SDL_Renderer *renderer, Sint16 x, Sint16 y1, Sint16 y2)
    {
        return SDL_RenderDrawLine(renderer, x, y1, x, y2);
        ;
    }
    inline int pixel(SDL_Renderer *renderer, Sint16 x, Sint16 y) { return SDL_RenderDrawPoint(renderer, x, y); }

    inline int _drawQuadrants(SDL_Renderer *renderer, Sint16 x, Sint16 y, Sint16 dx, Sint16 dy, Sint32 f)
    {
        int result = 0;
        Sint16 xpdx, xmdx;
        Sint16 ypdy, ymdy;

        if (points.size() == 0)
        {
            points.reserve(7);
        }
        else
        {
            points.clear();
        }

        if (dx == 0)
        {
            if (dy == 0)
            {
                // result |= pixel(renderer, x, y);
                points.push_back({x, y});
            }
            else
            {
                ypdy = y + dy;
                ymdy = y - dy;
                if (f)
                {
                    result |= vline(renderer, x, ymdy, ypdy);
                }
                else
                {
                    points.push_back({x, ypdy});
                    points.push_back({x, ymdy});
                    // result |= pixel(renderer, x, ypdy);
                    // result |= pixel(renderer, x, ymdy);
                }
            }
        }
        else
        {
            xpdx = x + dx;
            xmdx = x - dx;
            ypdy = y + dy;
            ymdy = y - dy;
            if (f)
            {
                result |= vline(renderer, xpdx, ymdy, ypdy);
                result |= vline(renderer, xmdx, ymdy, ypdy);
            }
            else
            {
                // result |= pixel(renderer, xpdx, ypdy);
                // result |= pixel(renderer, xmdx, ypdy);
                // result |= pixel(renderer, xpdx, ymdy);
                // result |= pixel(renderer, xmdx, ymdy);

                points.push_back({xpdx, ypdy});
                points.push_back({xmdx, ypdy});
                points.push_back({xpdx, ymdy});
                points.push_back({xmdx, ymdy});
            }
        }
        result |= SDL_RenderDrawPoints(renderer, points.data(), points.size());

        return result;
    }
    int hline(SDL_Renderer *renderer, Sint16 x1, Sint16 x2, Sint16 y)
    {
        return SDL_RenderDrawLine(renderer, x1, y, x2, y);
        ;
    }
/*!
\brief Internal function to draw ellipse or filled ellipse with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the ellipse.
\param y Y coordinate of the center of the ellipse.
\param rx Horizontal radius in pixels of the ellipse.
\param ry Vertical radius in pixels of the ellipse.
\param r The red value of the ellipse to draw.
\param g The green value of the ellipse to draw.
\param b The blue value of the ellipse to draw.
\param a The alpha value of the ellipse to draw.
\param f Flag indicating if the ellipse should be filled (1) or not (0).

\returns Returns 0 on success, -1 on failure.
            */
#define DEFAULT_ELLIPSE_OVERSCAN 4
    int _ellipseRGBA(SDL_Renderer *renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b,
                     Uint8 a, Sint32 f)
    {
        int result;
        Sint32 rxi, ryi;
        Sint32 rx2, ry2, rx22, ry22;
        Sint32 error;
        Sint32 curX, curY, curXp1, curYm1;
        Sint32 scrX, scrY, oldX, oldY;
        Sint32 deltaX, deltaY;
        Sint32 ellipseOverscan;

        /*
         * Sanity check radii
         */
        if ((rx < 0) || (ry < 0))
        {
            return (-1);
        }

        /*
         * Set color
         */
        result = 0;
        result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
        result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

        /*
         * Special cases for rx=0 and/or ry=0: draw a hline/vline/pixel
         */
        if (rx == 0)
        {
            if (ry == 0)
            {
                return (pixel(renderer, x, y));
            }
            else
            {
                return (vline(renderer, x, y - ry, y + ry));
            }
        }
        else
        {
            if (ry == 0)
            {
                return (hline(renderer, x - rx, x + rx, y));
            }
        }

        /*
         * Adjust overscan
         */
        rxi = rx;
        ryi = ry;
        if (rxi >= 512 || ryi >= 512)
        {
            ellipseOverscan = DEFAULT_ELLIPSE_OVERSCAN / 4;
        }
        else if (rxi >= 256 || ryi >= 256)
        {
            ellipseOverscan = DEFAULT_ELLIPSE_OVERSCAN / 2;
        }
        else
        {
            ellipseOverscan = DEFAULT_ELLIPSE_OVERSCAN / 1;
        }

        /*
         * Top/bottom center points.
         */
        oldX = scrX = 0;
        oldY = scrY = ryi;
        result |= _drawQuadrants(renderer, x, y, 0, ry, f);

        /* Midpoint ellipse algorithm with overdraw */
        rxi *= ellipseOverscan;
        ryi *= ellipseOverscan;
        rx2 = rxi * rxi;
        rx22 = rx2 + rx2;
        ry2 = ryi * ryi;
        ry22 = ry2 + ry2;
        curX = 0;
        curY = ryi;
        deltaX = 0;
        deltaY = rx22 * curY;

        /* Points in segment 1 */
        error = ry2 - rx2 * ryi + rx2 / 4;
        while (deltaX <= deltaY)
        {
            curX++;
            deltaX += ry22;

            error += deltaX + ry2;
            if (error >= 0)
            {
                curY--;
                deltaY -= rx22;
                error -= deltaY;
            }

            scrX = curX / ellipseOverscan;
            scrY = curY / ellipseOverscan;
            if ((scrX != oldX && scrY == oldY) || (scrX != oldX && scrY != oldY))
            {
                result |= _drawQuadrants(renderer, x, y, scrX, scrY, f);
                oldX = scrX;
                oldY = scrY;
            }
        }

        /* Points in segment 2 */
        if (curY > 0)
        {
            curXp1 = curX + 1;
            curYm1 = curY - 1;
            error = ry2 * curX * curXp1 + ((ry2 + 3) / 4) + rx2 * curYm1 * curYm1 - rx2 * ry2;
            while (curY > 0)
            {
                curY--;
                deltaY -= rx22;

                error += rx2;
                error -= deltaY;

                if (error <= 0)
                {
                    curX++;
                    deltaX += ry22;
                    error += deltaX;
                }

                scrX = curX / ellipseOverscan;
                scrY = curY / ellipseOverscan;
                if ((scrX != oldX && scrY == oldY) || (scrX != oldX && scrY != oldY))
                {
                    oldY--;
                    for (; oldY >= scrY; oldY--)
                    {
                        result |= _drawQuadrants(renderer, x, y, scrX, oldY, f);
                        /* prevent overdraw */
                        if (f)
                        {
                            oldY = scrY - 1;
                        }
                    }
                    oldX = scrX;
                    oldY = scrY;
                }
            }

            /* Remaining points in vertical */
            if (!f)
            {
                oldY--;
                for (; oldY >= 0; oldY--)
                {
                    result |= _drawQuadrants(renderer, x, y, scrX, oldY, f);
                }
            }
        }

        return (result);
    }

    int ellipseRGBA(SDL_Renderer *renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b,
                    Uint8 a)
    {
        return _ellipseRGBA(renderer, x, y, rx, ry, r, g, b, a, 0);
    }
    /*!
\brief Draw circle with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the circle.
\param y Y coordinate of the center of the circle.
\param rad Radius in pixels of the circle.
\param r The red value of the circle to draw.
\param g The green value of the circle to draw.
\param b The blue value of the circle to draw.
\param a The alpha value of the circle to draw.

\returns Returns 0 on success, -1 on failure.
            */
    int circleRGBA(SDL_Renderer *renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        return ellipseRGBA(renderer, x, y, rad, rad, r, g, b, a);
    }

    Renderer::Renderer() : camera(nullptr)
    {
        delta = 0;
        this->end = 0;
        lastTick = 0;
        ren = nullptr;
        viewPort.x = 0;
        viewPort.y = 0;
        viewPort.width = 0;
        viewPort.height = 0;
        zoomFactor = 1;
    }

    void Renderer::open(const GameWindow *pWin, bool pVSync)
    {

        const int numdrivers = SDL_GetNumRenderDrivers();
        SGL_LOG_INFO("Render driver count: " + std::to_string(numdrivers));

        unsigned int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
        if (pVSync)
        {
            renderFlags |= SDL_RENDERER_PRESENTVSYNC;
        }
        int drvId = -1;
        for (int i = 0; i < numdrivers; i++)
        {
            SDL_RendererInfo drinfo;
            SDL_GetRenderDriverInfo(i, &drinfo);
            SGL_LOG_TRACE("Driver name (" + std::to_string(i) + "): " + drinfo.name);
            if (drinfo.flags & SDL_RENDERER_SOFTWARE)
                SGL_LOG_TRACE(" the renderer is a software fallback");
            if (drinfo.flags & SDL_RENDERER_ACCELERATED)
                SGL_LOG_TRACE(" the renderer uses hardware acceleration");
            if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC)
                SGL_LOG_TRACE(" present is synchronized	with the refresh rate");
            if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE)
                SGL_LOG_TRACE(" the renderer supports rendering to texture");

            if (drinfo.flags & renderFlags)
            {
                drvId = i;
                break;
            }
        }
        if (pWin->getSettings()->getValueI("Base", "Driver") != 0)
        {
            drvId = pWin->getSettings()->getValueI("Base", "Driver");
        }
        else
        {
#ifdef __WIN32
            drvId = -1;
#endif
        }

        if (!SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1"))
        {
            SGL_LOG_ERROR_SDL();
        }

        SGL_LOG_TRACE("use driver id = " + std::to_string(drvId));

        ren = SDL_CreateRenderer(const_cast<SDL_Window *>(pWin->getSDLWindow()), drvId, renderFlags);

        if (ren == nullptr)
        {
            SGL_LOG_ERROR_SDL();
            throw SDLException("Renderer::open");
        }

        // set viewport
        graphics::Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.width = static_cast<float>(pWin->getWidth());
        rect.height = static_cast<float>(pWin->getHeight());
        setViewPort(rect);
    }
    Renderer::~Renderer()
    {
        if (ren != nullptr)
            SDL_DestroyRenderer(ren);

        points.clear();
    }

    SDL_Renderer *Renderer::getRenderer() { return ren; }
    void Renderer::clear() { SDL_RenderClear(ren); }

    void Renderer::renderPresent() { SDL_RenderPresent(ren); }

    void Renderer::calcDelta()
    {
        // calc time delta
        const Uint32 end = getTickCount();
        delta = end - lastTick;

        lastTick = end;
    }

    Uint32 Renderer::getTickCount() { return SDL_GetTicks(); }
    void Renderer::setRenderTargetInternal(SDL_Texture *pTexture)
    {
        if (SDL_SetRenderTarget(ren, pTexture) != 0)
        {
            SGL_LOG_ERROR_SDL();
            throw SDLException("SDL_SetRenderTarget");
        }
    }

    void Renderer::setRenderTarget(graphics::Texture *texture)
    {
        if (texture == nullptr)
            setRenderTargetInternal(nullptr);
        else
            setRenderTargetInternal(texture->getSDLTexture());
    }

    Uint32 Renderer::getTimeDelta() const { return delta; }

    const graphics::Rect &Renderer::getViewPort()
    {
        if (viewPort.width == 0.0f)
        {
            SDL_Rect rect;
            SDL_RenderGetViewport(ren, &rect);
            viewPort.width = static_cast<float>(rect.w);
            viewPort.height = static_cast<float>(rect.h);
            viewPort.x = static_cast<float>(rect.x);
            viewPort.y = static_cast<float>(rect.y);
        }
        return viewPort;
    }
    void Renderer::setViewPort(const graphics::Rect &rect)
    {
        viewPort = rect;
        SDL_Rect r = viewPort.sdlRect();
        if (SDL_RenderSetViewport(ren, &r) != 0)
        {
            SGL_LOG_ERROR_SDL();
            throw SDLException("SDL_RenderSetViewport");
        }
    }

    void Renderer::setMainCamera(Camera *pCamera) { camera = pCamera; }
    Camera *Renderer::getMainCamera() { return camera; }

    void Renderer::drawCircle(int x, int y, int rad, SDL_Color color)
    {
        int result = circleRGBA(ren, x, y, rad, color.r, color.g, color.b, color.a);
        if (result == -1)
        {
            SGL_LOG_ERROR_SDL();
            throw SDLException("drawCircle");
        }
    }

    void Renderer::drawPoint(float x, float y) { SDL_RenderDrawPointF(ren, x, y); }
    void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (SDL_SetRenderDrawColor(ren, r, g, b, a) != 0)
        {
            SGL_LOG_ERROR_SDL();
            throw SDLException("setDrawColor");
        }
    }

    void Renderer::setDrawColor(const SDL_Color &color) { setDrawColor(color.r, color.g, color.b, color.a); }
    void Renderer::drawRect(const graphics::Rect &rect)
    {
        sdlFRectP(rect, &cacheRect);
        if (SDL_RenderDrawRectF(ren, &cacheRect) != 0)
        {
            SGL_LOG_ERROR_SDL();
            throw SDLException("drawRect");
        }
    }

    void Renderer::fillRect(const graphics::Rect &rect)
    {
        sdlFRectP(rect, &cacheRect);
        int result = SDL_RenderFillRectF(ren, &cacheRect);
        if (result != 0)
        {
            SGL_LOG_ERROR_SDL();
            throw SDLException("fillRect");
        }
    }
    float Renderer::getZoomFactor() const { return zoomFactor; }
    void Renderer::setZoomFactor(float factor) { zoomFactor = factor; }

    void Renderer::setDrawBlendMode(SDL_BlendMode blendMode) { SDL_SetRenderDrawBlendMode(ren, blendMode); }

    SDL_BlendMode Renderer::getDrawBlendMode()
    {
        SDL_BlendMode blendMode;
        SDL_GetRenderDrawBlendMode(ren, &blendMode);
        return blendMode;
    }

    void Renderer::drawLine(utils::Vector2 &start, utils::Vector2 &end)
    {
        SDL_RenderDrawLineF(ren, start.getX(), start.getY(), end.getX(), end.getY());
    }

    void Renderer::updateViewportSizeByWindow(const GameWindow *pWin)
    {
        auto viewPort = getViewPort();
        viewPort.width = float(pWin->getWidth());
        viewPort.height = float(pWin->getHeight());
        setViewPort(viewPort);
    }

} // namespace core
// namespace core
