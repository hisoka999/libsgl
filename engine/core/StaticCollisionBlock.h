#pragma once

#include <string>
#include "engine/graphics/rect.h"

namespace core
{
    struct StaticCollisionBlock
    {
        graphics::Rect rect;
        void *physicsBody = nullptr;
        std::string blockData = "";
    };
} // namespace core
