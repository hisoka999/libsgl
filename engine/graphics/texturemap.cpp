#include "texturemap.h"
#include <engine/graphics/TextureManager.h>
#include <fstream>
#include <engine/utils/json/parser.h>

namespace graphics
{

    TextureMap::TextureMap()
    {
    }
    std::string TextureMap::getFileName()
    {
        return textureName;
    }
    void TextureMap::render(const std::string_view &subTexture, const graphics::Rect &destRect, core::Renderer *renderer)
    {
        texture->render(renderer, subTextures[hasher(subTexture)].rect, destRect);
    }

    void TextureMap::render(const size_t subTextureHash, const graphics::Rect &destRect, core::Renderer *renderer)
    {
        texture->render(renderer, subTextures[subTextureHash].rect, destRect);
    }
    void TextureMap::loadFromFile(const std::string &fileName)
    {
        std::ifstream file;
        std::istringstream is;
        std::string s;
        std::string group;

        file.open(fileName.c_str(), std::ios::in);
        if (!file.is_open())
        {
            throw IOException(fileName, "file does not exists");
        }
        std::string buffer((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        utils::JSON::Parser parser;
        auto jsonObject = parser.parseObject(buffer);
        textureName = jsonObject->getStringValue("textureName");
        texture = graphics::TextureManager::Instance().loadTexture(textureName);

        auto jsonArray = jsonObject->getArray("subTextures");

        file.close();
        for (auto subTex : jsonArray)
        {
            auto subTexObject = std::get<std::shared_ptr<utils::JSON::Object>>(subTex);
            graphics::Rect textureData;
            textureData.x = subTexObject->getFloatValue("x");
            textureData.y = subTexObject->getFloatValue("y");
            textureData.width = subTexObject->getFloatValue("width");
            textureData.height = subTexObject->getFloatValue("height");
            auto subTextureName = subTexObject->getStringValue("name");
            utils::Vector2 offset = {0.f, 0.f};
            if (subTexObject->hasAttribute("offset"))
            {
                auto offsetObject = subTexObject->getObjectValue("offset");
                offset = {offsetObject->getFloatValue("x"), offsetObject->getFloatValue("y")};
            }
            SubTexture subTexture = {textureData, offset};
            subTextures[hasher(subTextureName)] = subTexture;
        }
    }
    void TextureMap::getSourceRect(const std::string_view &subTexture, graphics::Rect *src)
    {
        auto &r = subTextures[hasher(subTexture)];
        src->x = r.rect.x;
        src->y = r.rect.y;
        src->width = r.rect.width;
        src->height = r.rect.height;
    }

    void TextureMap::getSourceRect(const size_t subTexture, graphics::Rect *src)
    {
        auto &r = subTextures[subTexture];
        src->x = r.rect.x;
        src->y = r.rect.y;
        src->width = r.rect.width;
        src->height = r.rect.height;
    }

    utils::Vector2 &TextureMap::getOffset(const std::string_view &subTexture)
    {
        return subTextures[hasher(subTexture)].offset;
    }

    utils::Vector2 &TextureMap::getOffset(const size_t subTexture)
    {
        return subTextures[subTexture].offset;
    }

    const std::shared_ptr<graphics::Texture> &TextureMap::getTexture()
    {
        return texture;
    }

}
