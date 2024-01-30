#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "engine/core/renderer.h"
#include "engine/graphics/rect.h"

namespace graphics
{

    class Texture
    {
    public:
        /** Default constructor */
        Texture();
        Texture(core::Renderer *pRenderer, const int pWidth, const int pHeight);
        Texture(core::Renderer *pRenderer, const int pWidth, const int pHeight, SDL_TextureAccess targetAccess);
        /** Default destructor */
        virtual ~Texture();
        void loadTexture(core::Renderer *ren, const std::string&filename);
        void render(core::Renderer *ren, int x, int y);
        void render(core::Renderer *ren, int x, int y, int pWidth, int pHeight, int pSrcX, int pSrcY);
        void render(core::Renderer *ren, const Rect &pSrc, const Rect &pDest);
        void renderResized(core::Renderer *ren, int x, int y, int pWidth, int pHeight);
        void renderRotated(core::Renderer *ren, const double angle, const int x, const int y, const int pWidth, const int pHeight);
        void setColorKey(uint8_t r, uint8_t g, uint8_t b);
        void setBlendMode(SDL_BlendMode blendMode);
        void setAlphaColor(core::Renderer *ren, uint8_t r, uint8_t g, uint8_t b);
        void setAlphaMod(uint8_t alpha);
        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;
        SDL_Texture *getSDLTexture() const;
        // Pixel manipulators
        bool lockTexture();
        bool unlockTexture();
        [[nodiscard]] void *getPixels() const;
        void copyPixels(const void *pixels);
        [[nodiscard]] int getPitch() const;
        Uint32 getPixel32(unsigned int x, unsigned int y);
        void setPixel(int x, int y, SDL_Color color);

        [[nodiscard]] Rect getTextureRect() const;

    protected:
    private:
        SDL_Texture *tex;
        SDL_Surface *surface;
        int width;
        int height;
        void *pixels;
        int pitch;
        SDL_PixelFormat *pixelFormat;

        // temp structure def
        SDL_FRect dst;
        SDL_Rect src;
    };

} // namespace graphics

#endif // GRAPHICS_TEXTURE_H
