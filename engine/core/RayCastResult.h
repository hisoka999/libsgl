#pragma once
#include <vector>
#include "RayCastHit.h"

namespace core
{
    namespace ecs
    {
        class Entity;
    } // namespace ecs

    struct RayCastResult
    {
        std::vector<RayCastHit> hits;

        operator bool() const
        {
            return !hits.empty();
        }
    };

} // namespace core
