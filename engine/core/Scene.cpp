/*
 * Scene.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "engine/core/Scene.h"
#include "engine/core/ecs/Entity.h"
#include "engine/core/ecs/Component.h"
#include <algorithm>
#include <execution>

namespace core
{

    Scene::Scene(core::Renderer *pRenderer)
        : renderer(pRenderer)
    {
        winMgr = std::make_shared<UI::WindowManager>();
        music = std::make_shared<core::Music>();
    }

    void Scene::unload()
    {
        if (music->isMusicPlaying())
        {
            music->stop();
        }
    }

    void Scene::load()
    {
    }

    void Scene::fixedUpdate([[maybe_unused]] uint32_t delta)
    {
    }

    void Scene::setGameWindow(core::GameWindow *gameWindow)
    {
        m_gameWindow = gameWindow;
    }

    core::ecs::Entity Scene::createEntity(const std::string &tagName)
    {
        auto handle = m_registry.create();
        core::ecs::Entity entity = {handle, this};
        entity.addComponent<core::ecs::TagComponent>(tagName);
        return entity;
    }

    void Scene::destroyEntity(core::ecs::Entity &entity)
    {
        m_registry.destroy(entity);
    }

    void Scene::renderEntities(core::Renderer *renderer)
    {
        auto cameraPosition = renderer->getMainCamera()->getPosition();
        {
            auto view = m_registry.group<core::ecs::RenderComponent, core::ecs::Transform>();
            graphics::Rect displayRect;
            for (auto &e : view)
            {

                auto &rendererComponent = view.get<core::ecs::RenderComponent>(e);
                auto transform = view.get<core::ecs::Transform>(e);
                transform.displayRect(displayRect);
                if (!displayRect.intersects(renderer->getMainCamera()->getViewPortRect()))
                    continue;
                transform.position -= cameraPosition;

                rendererComponent.render(transform, renderer);
            };
        }
        {
            auto view = m_registry.view<core::ecs::TextureMapAnimationRenderComponent, core::ecs::Transform>();

            std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, &renderer, &cameraPosition](auto e)
                          {
                // AnimationRenderComponent
                auto &rendererComponent = view.get<core::ecs::TextureMapAnimationRenderComponent>(e);
                auto transform = view.get<core::ecs::Transform>(e);
                transform.position -= cameraPosition;
                rendererComponent.animation.render(renderer, transform.position); });
        }
    }

    void Scene::fixedUpdateEntities(uint32_t delta)
    {
        auto rectPhysics = m_registry.view<core::ecs::RectangleCollider, core::ecs::ScriptComponent, core::ecs::Transform>();
        auto boxListPhysics = m_registry.view<core::ecs::BoxListCollider>();

        for (auto otherEntity : rectPhysics)
        {

            auto &otherCollider = rectPhysics.get<core::ecs::RectangleCollider>(otherEntity);

            for (auto e : rectPhysics)
            {
                if (otherEntity == e)
                    continue;

                auto [collider, script] = rectPhysics.get<core::ecs::RectangleCollider, core::ecs::ScriptComponent>(e);
                auto &transform = rectPhysics.get<core::ecs::Transform>(e);
                collider.body.position = transform.position;
                collider.body.checkCollision(&otherCollider.body);
            }
        }
        for (auto otherEntity : boxListPhysics)
        {
            auto &otherCollider = boxListPhysics.get<core::ecs::BoxListCollider>(otherEntity);
            for (auto &body : otherCollider.bodies)
            {
                auto oldPosition = body.position;
                // body.position = oldPosition - renderer->getMainCamera()->getPosition();
                for (auto e : rectPhysics)
                {
                    auto [collider, script] = rectPhysics.get<core::ecs::RectangleCollider, core::ecs::ScriptComponent>(e);
                    auto &transform = rectPhysics.get<core::ecs::Transform>(e);
                    collider.body.position = transform.position;
                    collider.body.checkCollision(&body);
                }
                body.position = oldPosition;
            }
        }

        for (auto e : rectPhysics)
        {
            auto [collider, script] = rectPhysics.get<core::ecs::RectangleCollider, core::ecs::ScriptComponent>(e);
            auto &transform = rectPhysics.get<core::ecs::Transform>(e);
            collider.body.position = transform.position;
            script.Instance->onUpdate(delta);
            std::vector<core::ecs::Entity> collisions;

            collider.body.Run(delta / 1000.0f);
            transform.position = collider.body.position;
        }

        {
            auto view = m_registry.view<core::ecs::TextureMapAnimationRenderComponent>();

            for (auto &e : view)
            {
                auto &animation = view.get<core::ecs::TextureMapAnimationRenderComponent>(e);
                animation.animation.update();
            }
        }
    }

    bool Scene::handleEventsEntities(core::Input *input)
    {
        auto view = m_registry.view<core::ecs::ScriptComponent>();
        bool result = false;
        for (auto &e : view)
        {
            if (!result)
            {
                auto &c = view.get<core::ecs::ScriptComponent>(e);
                result = c.Instance->onHandleInput(input);
            }
        }
        return result;
    }

    Scene::~Scene()
    {
        winMgr = nullptr;
        music = nullptr;
    }

    void Scene::update()
    {
    }

} /* namespace character */
