#ifndef CORE_SCENE_H_
#define CORE_SCENE_H_
#include <memory>
#include <optional>
#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/core/gamewindow.h"
#include "engine/graphics/texture.h"
#include "engine/ui/windowmanager.h"
#include <engine/core/Music.h>

#include "engine/core/ecs/entt.h"
#include "StaticCollisionBlock.h"

class b2World;
class b2ContactFilter;

namespace core::ecs
{
    class Entity;
}
namespace core
{

    class ContactListener;
    class RayCastCallback;
    class RayCastResult;

    enum FilterCategory : uint16_t
    {
        FC_NONE = 0,
        FC_ENTITY = 1,
        FC_STATIC_COLLIDER = 2
    };

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

        std::optional<core::ecs::Entity> findEntityByName(const std::string &tagName);
        void OnPhysics2DStart();
        void destoryEntity(const core::ecs::Entity &entity);
        void addStaticBlockCollider(std::vector<graphics::Rect> collider);
        void addStaticBlockCollider(std::vector<StaticCollisionBlock> collider);
        std::shared_ptr<UI::WindowManager> &getWindowManager();
        RayCastResult raycast(const utils::Vector2 &startPosition, const utils::Vector2 &endPosition);

    protected:
        void renderEntities(core::Renderer *renderer);
        void fixedUpdateEntities(uint32_t delta);
        bool handleEventsEntities(core::Input *input);
        void setPixelPerMeter(float value);
        core::Renderer *renderer;
        std::shared_ptr<UI::WindowManager> winMgr;
        std::shared_ptr<core::Music> music;
        core::GameWindow *m_gameWindow = nullptr;
        void setPhysicsDebug(bool debug);
        bool getPhysicsDebug();
        float pixelPerMeter = 50.f;
        float metersPerPixel = 0.02f;

    private:
        bool m_physicsDebug = false;
        void initPhysicsForEntity(entt::entity e);
        void initPhysicsForStaticCollider(StaticCollisionBlock &collider, size_t blockIndex);
        entt::registry m_registry;
        b2World *m_PhysicsWorld = nullptr;

        std::unique_ptr<ContactListener> contactListener;
        std::vector<entt::entity> m_entitiesToDestroy;
        std::vector<entt::entity> m_entitiesAdded;
        std::vector<StaticCollisionBlock> m_staticBlockCollider;

        friend class core::ecs::Entity;
        friend class RayCastCallback;
    };

} /* namespace core */

#endif /* CORE_SCENE_H_ */
