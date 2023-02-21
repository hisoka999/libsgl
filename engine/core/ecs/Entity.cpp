#include "Entity.h"
#include <engine/core/ecs/Component.h>

namespace core::ecs
{
    Entity::Entity(entt::entity id, Scene *scene)
        : m_handle(id), m_scene(scene)
    {
    }

    const std::string Entity::tagName() const
    {
        if (m_scene == nullptr)
            return "empty_entity";
        return m_scene->m_registry.get<core::ecs::TagComponent>(m_handle).TagName;
    }

    bool Entity::compareTag(const std::string &tag) const
    {
        if (m_scene)
            return m_scene->m_registry.get<core::ecs::TagComponent>(m_handle).TagName == tag;
        return false;
    }

    Entity::~Entity()
    {
    }
}