#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include <engine/utils/logger.h>

namespace graphics
{

    Texture::Texture()
        : width(0), height(0)
    {
        tex = nullptr;
        surface = nullptr;
        pixels = nullptr;
        pixelFormat = nullptr;
    }

    Texture::Texture(core::Renderer *pRenderer, const int pWidth, const int pHeight)
    {
        tex = SDL_CreateTexture(pRenderer->getRenderer(), SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_TARGET, pWidth, pHeight);

        width = pWidth;
        height = pHeight;
        surface = nullptr;
        pixels = nullptr;
        pixelFormat = nullptr;

        if (tex == nullptr)
        {
            throw SDLException("SDL_CreateTexture");
        }
    }

    Texture::Texture(core::Renderer *pRenderer, const int pWidth, const int pHeight, SDL_TextureAccess targetAccess)
    {
        tex = SDL_CreateTexture(pRenderer->getRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                targetAccess, pWidth, pHeight);

        width = pWidth;
        height = pHeight;
        surface = nullptr;
        pixels = nullptr;
        Uint32 format;

        if (tex == nullptr)
        {
            throw SDLException("SDL_CreateTexture");
        }
        SDL_QueryTexture(tex, &format, nullptr, &width, &height);
        pixelFormat = SDL_AllocFormat(format);
    }

    void Texture::loadTexture(core::Renderer *ren, std::string filename)
    {
        SGL_LOG_TRACE("load texture: " + filename);
        surface = IMG_Load(filename.c_str());

        if (surface == nullptr)
        {

            SGL_LOG_ERROR_SDL();
            throw SDLException("IMG_LoadTexture");
        }
        tex = SDL_CreateTextureFromSurface(ren->getRenderer(), surface);
        // Query the texture to get its width and height to use
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
    }
    void Texture::render(core::Renderer *ren, int x, int y)
    {
        render(ren, x, y, width, height, 0, 0);
    }

    void Texture::renderResized(core::Renderer *ren, int x, int y, int pWidth,
                                int pHeight)
    {
        SDL_FRect dst;
        dst.x = float(x);
        dst.y = float(y);
        dst.w = float(pWidth);
        dst.h = float(pHeight);

        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = width;
        src.h = height;
        SDL_RenderCopyF(ren->getRenderer(), tex, &src, &dst);
    }

    void Texture::render(core::Renderer *ren, const Rect &pSrc, const Rect &pDest)
    {

        core::sdlRectP(pSrc, &src);
        core::sdlFRectP(pDest, &dst);
        SDL_RenderCopyF(ren->getRenderer(), tex, &src, &dst);
    }

    void Texture::render(core::Renderer *ren, int x, int y, int pWidth, int pHeight,
                         int pSrcX, int pSrcY)
    {
        SDL_FRect dst;
        dst.x = float(x);
        dst.y = float(y);
        dst.w = float(pWidth);
        dst.h = float(pHeight);

        SDL_Rect src;
        src.x = pSrcX;
        src.y = pSrcY;
        src.w = pWidth;
        src.h = pHeight;

        SDL_RenderCopyF(ren->getRenderer(), tex, &src, &dst);
    }

    void Texture::renderRotated(core::Renderer *ren, const double angle,
                                const int x, const int y, const int pWidth, const int pHeight)
    {
        SDL_FPoint *center = nullptr;

        SDL_FRect dst;
        dst.x = float(x);
        dst.y = float(y);
        dst.w = float(pWidth);
        dst.h = float(pHeight);

        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = width;
        src.h = height;

        int result = SDL_RenderCopyExF(ren->getRenderer(), tex, &src, &dst, angle, center,
                                       SDL_FLIP_NONE);
        if (result != 0)
            throw SDLException("renderRotated::SDL_RenderCopyEx");
    }

    int Texture::getWidth()
    {
        return width;
    }
    int Texture::getHeight()
    {
        return height;
    }

    SDL_Texture *Texture::getSDLTexture()
    {
        return tex;
    }

    bool Texture::lockTexture()
    {
        bool success = true;

        // Texture is already locked
        if (pixels != nullptr)
        {
            SGL_LOG_ERROR("Texture is already locked!");
            success = false;
        }
        // Lock texture
        else
        {
            if (SDL_LockTexture(tex, nullptr, &pixels, &pitch) != 0)
            {
                SGL_LOG_ERROR(std::string("Unable to lock texture! ") + SDL_GetError());
                success = false;
            }
        }

        return success;
    }

    bool Texture::unlockTexture()
    {
        bool success = true;

        // Texture is not locked
        if (pixels == nullptr)
        {
            SGL_LOG_ERROR("Texture is not locked!\n");
            success = false;
        }
        // Unlock texture
        else
        {
            SDL_UnlockTexture(tex);
            pixels = nullptr;
            pitch = 0;
        }

        return success;
    }

    void *Texture::getPixels()
    {
        return pixels;
    }

    void Texture::copyPixels(void *pixels)
    {
        // Texture is locked
        if (pixels != NULL)
        {
            // Copy to locked pixels
            memcpy(this->pixels, pixels, pitch * height);
        }
    }

    int Texture::getPitch()
    {
        return pitch;
    }

    Uint32 Texture::getPixel32(unsigned int x, unsigned int y)
    {
        // Convert the pixels to 32 bit
        Uint32 *_pixels = (Uint32 *)this->pixels;

        // Get the pixel requested
        return _pixels[(y * (pitch / 4)) + x];
    }

    void Texture::setPixel(int x, int y, SDL_Color color)
    {

        Uint32 *_pixels = (Uint32 *)this->pixels;
        Uint32 value = SDL_MapRGBA(pixelFormat, color.r, color.g, color.b, color.a);
        _pixels[(y * (pitch / 4)) + x] = value;
    }

    graphics::Rect Texture::getTextureRect()
    {
        return {0, 0, float(width), float(height)};
    }
    Texture::~Texture()
    {
        SDL_DestroyTexture(tex);
        if (surface != nullptr)
            SDL_FreeSurface(surface);
        surface = nullptr;
        if (pixelFormat != nullptr)
            SDL_FreeFormat(pixelFormat);
    }

    void Texture::setColorKey(uint8_t r, uint8_t g, uint8_t b)
    {
        SDL_SetTextureColorMod(tex, r, g, b);
    }
    void Texture::setBlendMode(SDL_BlendMode blendMode)
    {
        SDL_SetTextureBlendMode(tex, blendMode);
    }
    void Texture::setAlphaColor(core::Renderer *ren, uint8_t r, uint8_t g, uint8_t b)
    {
        if (tex != nullptr)
        {
            SDL_DestroyTexture(tex);
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));

        tex = SDL_CreateTextureFromSurface(ren->getRenderer(), surface);
    }

    void Texture::setAlphaMod(uint8_t alpha)
    {
        SDL_SetTextureAlphaMod(tex, alpha);
    }

} // namespace graphics
