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
    void TextureMap::render(const std::string &subTexture, const graphics::Rect &destRect, core::Renderer *renderer)
    {
        texture->render(renderer, subTextures[subTexture], destRect);
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
            subTextures[subTextureName] = textureData;
        }
    }
    const graphics::Rect TextureMap::getSourceRect(const std::string &subTexture)
    {
        return subTextures[subTexture];
    }

}
