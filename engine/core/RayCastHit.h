#pragma once
#include "engine/core/ecs/Entity.h"
#include "engine/utils/vector2.h"
#include "StaticCollisionBlock.h"
#include <optional>

namespace core
{
    class RayCastHit
    {
    private:
        std::optional<core::ecs::Entity> m_entity;
        std::optional<StaticCollisionBlock> m_collisionBlock;
        utils::Vector2 m_position;

    public:
        RayCastHit(std::optional<core::ecs::Entity> entity, std::optional<StaticCollisionBlock> collisionBlock, utils::Vector2 position);
        ~RayCastHit();

        std::optional<core::ecs::Entity> &getEntity();
        std::optional<StaticCollisionBlock> &getCollisionBlock();
        utils::Vector2 &getPosition();

        operator bool() const;
    };

} // namespace core
