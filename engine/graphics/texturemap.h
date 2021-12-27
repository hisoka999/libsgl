#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include <engine/graphics/texture.h>
#include <memory>
#include <engine/utils/IniBase.h>
#include <engine/core/renderer.h>
#include <unordered_map>

namespace graphics
{

    class TextureMap
    {
    public:
        TextureMap();
        void loadFromFile(const std::string &fileName);
        void render(const std::string_view &subTexture, const graphics::Rect &destRect, core::Renderer *renderer);
        void render(const size_t subTextureHash, const graphics::Rect &destRect, core::Renderer *renderer);
        std::string getFileName();
        void getSourceRect(const std::string_view &subTexture, graphics::Rect *src);
        void getSourceRect(const size_t subTexture, graphics::Rect *src);
        const std::shared_ptr<graphics::Texture> &getTexture();

    private:
        std::string textureName;
        std::shared_ptr<graphics::Texture> texture;
        std::unordered_map<size_t, graphics::Rect> subTextures;
        std::hash<std::string_view> hasher;
    };

}

#endif // TEXTUREMAP_H
