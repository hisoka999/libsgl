#include "Component.h"

namespace core::ecs
{
    template <typename T>
    ScriptComponent &addScriptComponent(Entity &entity)
    {
        ScriptComponent component;
        component.bindAndCreate<T>();
        component.Instance->setEntity(entity);
        if (entity.HasComponent<ScriptComponentList>())
        {
            auto &list = entity.findComponent<ScriptComponentList>();
            return list.components.emplace_back(component);
        }
        else
        {
            auto &list = entity.addComponent<ScriptComponentList>();
            return list.components.emplace_back(component);
        }
    }

    template <typename T>
    T *findScriptComponent(Entity &entity)
    {
        size_t id = typeid(T).hash_code();
        auto &list = entity.findComponent<ScriptComponentList>();
        for (auto &script : list.components)
        {
            if (script.typeinfo == id)
            {
                return (T *)script.Instance;
            }
        }
        return nullptr;
    }

} // namespace core::ecs
