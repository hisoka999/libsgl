#pragma once

#include <string>
#include "engine/graphics/rect.h"

namespace core
{
    struct StaticCollisionBlock
    {
        graphics::Rect rect;
        void *physicsBody = nullptr;
        size_t blockId = 0;
        std::string blockData = "";
    };
} // namespace core
