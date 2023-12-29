#include "RayCastHit.h"

namespace core
{
    RayCastHit::RayCastHit(std::optional<core::ecs::Entity> entity, std::optional<StaticCollisionBlock> collisionBlock, utils::Vector2 position) : m_entity(entity), m_collisionBlock(collisionBlock), m_position(position)
    {
    }

    RayCastHit::~RayCastHit()
    {
    }

    RayCastHit::operator bool() const
    {
        return m_entity.has_value() || m_collisionBlock.has_value();
    }

    std::optional<core::ecs::Entity> &RayCastHit::getEntity()
    {
        return m_entity;
    }

    std::optional<StaticCollisionBlock> &RayCastHit::getCollisionBlock()
    {
        return m_collisionBlock;
    }

    utils::Vector2 &RayCastHit::getPosition()
    {
        return m_position;
    }
} // namespace core
