#pragma once
#include <cstdint>
#include "engine/core/Scene.h"

namespace core::ecs
{

    typedef uint64_t EntityId;

    class Entity
    {
    private:
        entt::entity m_handle;
        Scene *m_scene;

    public:
        Entity(entt::entity id, Scene *scene);
        ~Entity();
        Entity(const Entity &other) = default;
        Entity() = default;

        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        }
        template <typename T>
        bool HasComponent()
        {
            return m_scene->m_registry.has<T>(m_handle);
        }
        template <typename T>
        T &findComponent()
        {
            return m_scene->m_registry.get<T>(m_handle);
        }
        operator entt::entity() const { return m_handle; }
    };

}
