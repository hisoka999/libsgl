#ifndef GRAPHICS_TEXT_H
#define GRAPHICS_TEXT_H

#include <SDL2/SDL_ttf.h>
#include <string>
#include <engine/core/renderer.h>

namespace graphics
{
    enum class FontStyle : int
    {
        NORMAL = 0x00,
        BOLD = 0x01,
        ITALIC = 0x02,
        UNDERLINE = 0x04,
        STRIKETHROUGH = 0x08
    };
    class Text
    {
    public:
        /** Default constructor */
        Text();
        /** Default destructor */
        virtual ~Text();
        Text(const Text &org);

        void openFont(const std::string fontFile, int fontSize);
        void render(core::Renderer *ren, const std::string &message, SDL_Color color,
                    int x, int y);
        int size(const std::string &message, int *w, int *h);
        void renderWrapped(core::Renderer *ren, const std::string &message,
                           SDL_Color color, int x, int y, size_t lineLength);
        void setStyle(FontStyle style);
        FontStyle getStyle();

    protected:
    private:
        size_t genTextHash(const std::string &message, SDL_Color color);
        TTF_Font *font;
        std::map<size_t, SDL_Texture *> textCache;
        std::string fontFile;
        SDL_Rect dst;
        SDL_Rect src;
    };

} // namespace graphics

#endif // GRAPHICS_TEXT_H
