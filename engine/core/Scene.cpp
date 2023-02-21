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
#include "box2d/box2d.h"

namespace core
{

    class ContactListener : public b2ContactListener
    {
        Scene *scene;

    public:
        ContactListener(Scene *scene) : scene(scene) {}

        /// Called when two fixtures begin to touch.
        virtual void BeginContact(b2Contact *contact) override
        {
            core::ecs::Entity entityA = {(entt::entity)contact->GetFixtureA()->GetBody()->GetUserData().pointer, scene};
            core::ecs::Entity entityB = {(entt::entity)contact->GetFixtureB()->GetBody()->GetUserData().pointer, scene};
            bool existsScriptA = entityA.HasComponent<core::ecs::ScriptComponent>();
            bool existsScriptB = entityB.HasComponent<core::ecs::ScriptComponent>();

            if (existsScriptA)
            {
                auto &scriptA = entityA.findComponent<core::ecs::ScriptComponent>();
                scriptA.Instance->beginCollision(core::ecs::Collision{entityB});
            }
            if (existsScriptB)
            {
                auto &scriptB = entityB.findComponent<core::ecs::ScriptComponent>();
                scriptB.Instance->beginCollision(core::ecs::Collision{entityA});
            }
        }

        /// Called when two fixtures cease to touch.
        virtual void EndContact(b2Contact *contact) override
        {
            core::ecs::Entity entityA = {(entt::entity)contact->GetFixtureA()->GetBody()->GetUserData().pointer, scene};
            core::ecs::Entity entityB = {(entt::entity)contact->GetFixtureB()->GetBody()->GetUserData().pointer, scene};
            bool existsScriptA = entityA.HasComponent<core::ecs::ScriptComponent>();
            bool existsScriptB = entityB.HasComponent<core::ecs::ScriptComponent>();

            if (existsScriptA)
            {
                auto &scriptA = entityA.findComponent<core::ecs::ScriptComponent>();
                scriptA.Instance->endCollision(core::ecs::Collision{entityB});
            }
            if (existsScriptB)
            {
                auto &scriptB = entityB.findComponent<core::ecs::ScriptComponent>();
                scriptB.Instance->endCollision(core::ecs::Collision{entityA});
            }
        }
    };

    inline b2Vec2 convert2Meters(const float factor, const utils::Vector2 &input)
    {
        return b2Vec2(factor * input.getX(), factor * input.getY());
    }

    inline utils::Vector2 convertToPixels(const float factor, const b2Vec2 &input)
    {
        return utils::Vector2(factor * input.x, factor * input.y);
    }

    inline b2BodyType Rigidbody2DTypeToBox2DBody(core::ecs::Rigidbody2DComponent::BodyType bodyType)
    {
        switch (bodyType)
        {
        case core::ecs::Rigidbody2DComponent::BodyType::Static:
            return b2_staticBody;
        case core::ecs::Rigidbody2DComponent::BodyType::Dynamic:
            return b2_dynamicBody;
        case core::ecs::Rigidbody2DComponent::BodyType::Kinematic:
            return b2_kinematicBody;
        }

        return b2_staticBody;
    }

    inline core::ecs::Rigidbody2DComponent::BodyType Rigidbody2DTypeFromBox2DBody(b2BodyType bodyType)
    {
        switch (bodyType)
        {
        case b2_staticBody:
            return core::ecs::Rigidbody2DComponent::BodyType::Static;
        case b2_dynamicBody:
            return core::ecs::Rigidbody2DComponent::BodyType::Dynamic;
        case b2_kinematicBody:
            return core::ecs::Rigidbody2DComponent::BodyType::Kinematic;
        }

        return core::ecs::Rigidbody2DComponent::BodyType::Static;
    }

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
            m_registry.sort<core::ecs::Transform>([](const auto &lhs, const auto &rhs)
                                                  { return lhs.position.getY() + lhs.height < rhs.position.getY() + rhs.height; });
            auto group = m_registry.view<core::ecs::Transform>();

            graphics::Rect displayRect;
            for (auto &e : group)
            {

                auto transform = group.get<core::ecs::Transform>(e);
                transform.displayRect(displayRect);
                if (!displayRect.intersects(renderer->getMainCamera()->getViewPortRect()))
                    continue;
                transform.position -= cameraPosition;
                if (m_registry.has<core::ecs::RenderComponent>(e))
                {
                    auto &rendererComponent = m_registry.get<core::ecs::RenderComponent>(e);
                    rendererComponent.render(transform, renderer);
                }
                else if (m_registry.has<core::ecs::TextureMapAnimationRenderComponent>(e))
                {
                    auto &rendererComponent = m_registry.get<core::ecs::TextureMapAnimationRenderComponent>(e);
                    rendererComponent.animation.render(renderer, transform.position);
                }
            }
        }
    }
    void Scene::initPhysicsForEntity(entt::entity e)
    {
        core::ecs::Entity entity = {e, this};
        auto &transform = entity.findComponent<core::ecs::Transform>();
        auto &rb2d = entity.findComponent<core::ecs::Rigidbody2DComponent>();

        b2BodyDef bodyDef;
        bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
        bodyDef.position = convert2Meters(metersPerPixel, transform.position);
        bodyDef.angle = 0;

        b2Body *body = m_PhysicsWorld->CreateBody(&bodyDef);
        body->SetFixedRotation(rb2d.FixedRotation);
        body->GetUserData().pointer = (uintptr_t)e;
        rb2d.RuntimeBody = body;

        if (entity.HasComponent<core::ecs::BoxCollider2DComponent>())
        {
            auto &bc2d = entity.findComponent<core::ecs::BoxCollider2DComponent>();

            b2PolygonShape boxShape;
            boxShape.SetAsBox(bc2d.Size.getX() * transform.Scale.getX(), bc2d.Size.getY() * transform.Scale.getY(), {bc2d.Offset.getX(), bc2d.Offset.getY()}, 0);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.density = bc2d.Density;
            fixtureDef.friction = bc2d.Friction;
            fixtureDef.restitution = bc2d.Restitution;
            fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
            body->CreateFixture(&fixtureDef);
        }

        if (entity.HasComponent<core::ecs::CircleCollider2DComponent>())
        {
            auto &cc2d = entity.findComponent<core::ecs::CircleCollider2DComponent>();

            b2CircleShape circleShape;
            circleShape.m_p.Set(cc2d.Offset.getX(), cc2d.Offset.getY());
            circleShape.m_radius = transform.Scale.getX() * cc2d.Radius;

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circleShape;
            fixtureDef.density = cc2d.Density;
            fixtureDef.friction = cc2d.Friction;
            fixtureDef.restitution = cc2d.Restitution;
            fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
            body->CreateFixture(&fixtureDef);
        }
    }
    void Scene::OnPhysics2DStart()
    {
        m_PhysicsWorld = new b2World({0.0f, 0.0f});
        contactListener = std::make_unique<core::ContactListener>(this);
        m_PhysicsWorld->SetContactListener(contactListener.get());

        auto view = m_registry.view<core::ecs::Rigidbody2DComponent>();
        for (auto e : view)
        {
            initPhysicsForEntity(e);
        }
    }

    void Scene::fixedUpdateEntities(uint32_t delta)
    {

        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        m_PhysicsWorld->Step(delta / 1000.f, velocityIterations, positionIterations);

        // Retrieve transform from Box2D
        auto view = m_registry.view<core::ecs::Rigidbody2DComponent>();
        for (auto e : view)
        {

            core::ecs::Entity entity = {e, this};
            bool hasScript = entity.HasComponent<core::ecs::ScriptComponent>();
            if (hasScript)
            {
                auto &script = m_registry.get<core::ecs::ScriptComponent>(e);
                script.Instance->onUpdate(delta);
            }
            auto &transform = entity.findComponent<core::ecs::Transform>();
            auto &rb2d = entity.findComponent<core::ecs::Rigidbody2DComponent>();

            b2Body *body = (b2Body *)rb2d.RuntimeBody;
            transform.position = convertToPixels(pixelPerMeter, body->GetPosition());
            // transform.Rotation.z = body->GetAngle();
            if (hasScript)
            {
                auto &script = m_registry.get<core::ecs::ScriptComponent>(e);
                script.Instance->onUpdate(delta);

                for (b2ContactEdge *edge = body->GetContactList(); edge; edge = edge->next)
                {

                    entt::entity other = (entt::entity)edge->other->GetUserData().pointer;
                    core::ecs::Entity o = {other, this};
                    auto &otherScript = o.findComponent<core::ecs::ScriptComponent>();
                    script.Instance->onCollision(otherScript.Instance);
                }
            }
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

        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    void Scene::destoryEntity(const core::ecs::Entity &entity)
    {
        m_entitiesToDestroy.push_back(entity);
    }

    void Scene::update()
    {
        auto view = m_registry.view<core::ecs::ScriptComponent>();
        for (auto handle : view)
        {
            auto &script = view.get<core::ecs::ScriptComponent>(handle);
            if (script.Instance == nullptr)
            {
                core::ecs::Entity entity = {handle, this};
                script.Instance = script.InstantiateScript();
                script.Instance->setEntity(entity);
            }

            if (m_registry.has<core::ecs::Rigidbody2DComponent>(handle))
            {
                if (m_registry.get<core::ecs::Rigidbody2DComponent>(handle).RuntimeBody == nullptr)
                    initPhysicsForEntity(handle);
            }
        }

        for (auto &e : m_entitiesToDestroy)
        {
            if (m_registry.has<core::ecs::Rigidbody2DComponent>(e))
            {
                auto &rb2d = m_registry.get<core::ecs::Rigidbody2DComponent>(e);
                m_PhysicsWorld->DestroyBody((b2Body *)rb2d.RuntimeBody);
            }
            m_registry.destroy(e);
        }
        m_entitiesToDestroy.clear();
    }
    std::optional<core::ecs::Entity> Scene::findEntityByName(const std::string &tagName)
    {
        auto view = m_registry.view<core::ecs::TagComponent>();
        for (auto handle : view)
        {
            auto &tagComponent = view.get<core::ecs::TagComponent>(handle);
            if (tagComponent.TagName == tagName)
                return std::optional<core::ecs::Entity>{core::ecs::Entity{handle, this}};
        }
        return std::optional<core::ecs::Entity>{};
    }

    void Scene::setPixelPerMeter(float value)
    {
        pixelPerMeter = value;
        metersPerPixel = 1.f / value;
    }
} /* namespace character */
