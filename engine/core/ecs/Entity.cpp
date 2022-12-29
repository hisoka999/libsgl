#include "Entity.h"

namespace core::ecs
{
    Entity::Entity(entt::entity id, Scene *scene)
        : m_handle(id), m_scene(scene)
    {
    }

    Entity::~Entity()
    {
    }
}