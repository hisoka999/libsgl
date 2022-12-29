#ifndef CORE_SCENE_H_
#define CORE_SCENE_H_

#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/core/gamewindow.h"
#include "engine/graphics/texture.h"
#include "engine/ui/windowmanager.h"
#include <engine/core/Music.h>
#include <memory>
#include "engine/core/ecs/entt.h"

namespace core::ecs
{
    class Entity;
}
namespace core
{

    class Scene
    {
    public:
        Scene(core::Renderer *pRenderer);
        virtual ~Scene();

        virtual void render() = 0;
        virtual bool handleEvents(core::Input *pInput) = 0;
        virtual void update();
        virtual void unload();
        virtual void load();
        virtual void fixedUpdate(uint32_t delta);
        void setGameWindow(core::GameWindow *gameWindow);

        core::ecs::Entity createEntity(const std::string &tagName = "");
        void destroyEntity(core::ecs::Entity &entity);

        template <typename Component>
        std::vector<core::ecs::Entity> findEntitesByComponent()
        {
            std::vector<core::ecs::Entity> result;
            for (auto handle : m_registry.view<Component>())
            {
                result.push_back({handle, this});
            }
            return result;
        }

    protected:
        void renderEntities(core::Renderer *renderer);
        void fixedUpdateEntities(uint32_t delta);
        bool handleEventsEntities(core::Input *input);
        core::Renderer *renderer;
        std::shared_ptr<UI::WindowManager> winMgr;
        std::shared_ptr<core::Music> music;
        core::GameWindow *m_gameWindow = nullptr;

    private:
        entt::registry m_registry;

        friend class core::ecs::Entity;
    };

} /* namespace core */

#endif /* CORE_SCENE_H_ */
