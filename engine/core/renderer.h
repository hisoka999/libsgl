#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include <SDL2/SDL_render.h>
#include "engine/core/Camera.h"
#include "engine/core/gamewindow.h"
#include "engine/graphics/rect.h"

namespace graphics
{
    class Texture;
}

namespace core
{
    void sdlRectP(const graphics::Rect &src, SDL_Rect *r);
    void sdlFRectP(const graphics::Rect &src, SDL_FRect *r);
    class Renderer
    {
    public:
        /** Default constructor */
        Renderer();
        /** Default destructor */
        virtual ~Renderer();

        void open(const GameWindow *pWin, bool pVSync);

        SDL_Renderer *getRenderer();
        void clear();
        void renderPresent();
        Uint32 getTickCount();
        Uint32 getTimeDelta() const;

        void setRenderTarget(graphics::Texture *texture);
        const graphics::Rect &getViewPort();
        void setViewPort(const graphics::Rect &rect);
        void setMainCamera(Camera *pCamera);
        Camera *getMainCamera();
        void calcDelta();
        float getZoomFactor() const;
        void setZoomFactor(float factor);
        void setDrawBlendMode(SDL_BlendMode blendMode);
        SDL_BlendMode getDrawBlendMode();

        // drawing routines
        /**
         * draws a not filled circle on the renderer
         * @param
         * @param color of the ellipse
         * @throws
         */
        void drawCircle(int x, int y, int rad, SDL_Color color);
        void drawPoint(float x, float y);

        void drawRect(const graphics::Rect &rect);

        void fillRect(const graphics::Rect &rect);

        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void setDrawColor(const SDL_Color &color);

        void drawLine(utils::Vector2 &start, utils::Vector2 &end);
        void updateViewportSizeByWindow(const GameWindow *pWin);

    protected:
    private:
        void setRenderTargetInternal(SDL_Texture *pTexture);
        SDL_Renderer *ren;
        Uint32 lastTick;
        Uint32 end;
        Uint32 delta;
        graphics::Rect viewPort;
        Camera *camera;
        float zoomFactor;
        SDL_FRect cacheRect{};
    };

} // namespace core

#endif // CORE_RENDERER_H
