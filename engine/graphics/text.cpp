#include "engine/graphics/text.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/logger.h"
#include "engine/utils/color.h"

namespace graphics
{

    Text::Text()
    {
        // ctor
        font = nullptr;
    }

    Text::~Text()
    {
        for (auto surf : textCache)
        {
            SDL_DestroyTexture(surf.second);
        }
        textCache.clear();

        SGL_LOG_TRACE("delete font" + fontFile);
        if (font != nullptr)
        {
            SGL_LOG_TRACE("ttf was initialised: " + std::to_string(TTF_WasInit()));
            TTF_CloseFont(font);
            font = nullptr;
        }
    }

    Text::Text([[maybe_unused]] const Text &org)
    {
        font = nullptr;
        this->fontFile = "";
    }

    void Text::openFont(const std::string fontFile, int fontSize)
    {
        this->fontFile = fontFile + std::string(":" + std::to_string(fontSize));
        // Open the font
        SGL_LOG_TRACE("open font: " + this->fontFile);
        font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (font == nullptr)
        {
            SGL_LOG_ERROR_SDL();
            throw new SDLException("TTF_OpenFont");
        }
    }

    int Text::size(const std::string &message, int *w, int *h)
    {
        if (font != nullptr)
            return TTF_SizeUTF8(font, message.c_str(), w, h);
        else
            return -1;
    }

    size_t Text::genTextHash(const std::string &message, SDL_Color color)
    {

        unsigned long hash = 5381;
        int c;

        for (size_t pos = 0; pos < message.length(); ++pos)
        {
            c = message[pos];
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }

        return ((hash << 5) + hash) + utils::color::color2hex(color);
    }

    void Text::renderWrapped(core::Renderer *ren, const std::string &message,
                             SDL_Color color, int x, int y, size_t lineLength)
    {
        if (message.size() == 0)
            return;

        auto hash = genTextHash(message, color);

        SDL_Surface *surf = nullptr;
        SDL_Texture *texture = nullptr;
        if (textCache.count(hash) > 0)
        {
            texture = textCache[hash];
        }
        else
        {
            // We need to first render to a surface as that's what TTF_RenderText
            // returns, then load that surface into a texture
            int w, h;
            if (size(message, &w, &h) == -1)
            {
                throw new SDLException("TTF_SizeText");
            }
            if (w == 0)
            {
                return;
            }

            surf = TTF_RenderUTF8_Blended_Wrapped(font, message.c_str(),
                                                  color, lineLength);

            if (surf == nullptr)
            {
                TTF_GetError();
                SGL_LOG_ERROR_SDL();
                TTF_CloseFont(font);
                font = nullptr;
                throw new SDLException("TTF_RenderText_Blended");
            }
            texture = SDL_CreateTextureFromSurface(ren->getRenderer(), surf);
            if (texture == nullptr)
            {
                SGL_LOG_ERROR_SDL();
                throw new SDLException("SDL_CreateTextureFromSurface");
            }
            textCache[hash] = texture;
        }

        int iW, iH;
        SDL_QueryTexture(texture, nullptr, nullptr, &iW, &iH);

        dst.x = x;
        dst.y = y;
        dst.w = iW;
        dst.h = iH;

        src.x = 0;
        src.y = 0;
        src.w = iW;
        src.h = iH;

        SDL_RenderCopy(ren->getRenderer(), texture, &src, &dst);
        if (surf != nullptr)
        {
            SDL_FreeSurface(surf);
        }
        texture = nullptr;
    }

    void Text::setStyle(FontStyle style)
    {
        TTF_SetFontStyle(font, int(style));
    }

    void Text::render(core::Renderer *ren, const std::string &message,
                      SDL_Color color, int x, int y)
    {
        if (message.size() == 0)
            return;

        auto hash = genTextHash(message, color);
        SDL_Surface *surf = nullptr;
        SDL_Texture *texture = nullptr;
        if (textCache.count(hash) > 0)
        {
            texture = textCache[hash];
        }
        else
        {
            // We need to first render to a surface as that's what TTF_RenderText
            // returns, then load that surface into a texture
            int w, h;
            if (size(message, &w, &h) == -1)
            {
                throw new SDLException("TTF_SizeText");
            }
            if (w == 0)
            {
                return;
            }

            surf = TTF_RenderUTF8_Blended(font, message.c_str(),
                                          color);

            if (surf == nullptr)
            {
                TTF_GetError();
                SGL_LOG_ERROR_SDL();
                TTF_CloseFont(font);
                font = nullptr;
                throw new SDLException("TTF_RenderText_Blended");
            }
            texture = SDL_CreateTextureFromSurface(ren->getRenderer(), surf);
            if (texture == nullptr)
            {
                SGL_LOG_ERROR_SDL();
                throw new SDLException("SDL_CreateTextureFromSurface");
            }
            textCache[hash] = texture;
        }

        int iW, iH;
        SDL_QueryTexture(texture, nullptr, nullptr, &iW, &iH);

        dst.x = x;
        dst.y = y;
        dst.w = iW;
        dst.h = iH;

        src.x = 0;
        src.y = 0;
        src.w = iW;
        src.h = iH;

        SDL_RenderCopy(ren->getRenderer(), texture, &src, &dst);
        if (surf != nullptr)
        {
            SDL_FreeSurface(surf);
        }
        texture = nullptr;
    }

} // namespace graphics
