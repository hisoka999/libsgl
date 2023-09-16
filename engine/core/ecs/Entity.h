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
        Scene *m_scene = nullptr;

    public:
        Entity(entt::entity id, Scene *scene);
        ~Entity();
        Entity(const Entity &other) = default;
        Entity() = default;
        const std::string tagName() const;
        bool compareTag(const std::string &tag) const;

        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        }
        template <typename T>
        bool HasComponent()
        {
            return m_scene->m_registry.any_of<T>(m_handle);
        }
        template <typename T>
        T &findComponent()
        {
            return m_scene->m_registry.get<T>(m_handle);
        }
        template <typename T>
        const T &findComponent() const
        {
            return m_scene->m_registry.get<const T>(m_handle);
        }

        operator entt::entity() const { return m_handle; }
        Scene *getScene()
        {
            return m_scene;
        }

        operator bool() const { return m_scene != nullptr; };
    };

}
