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
#include "RayCastResult.h"

namespace core
{

    inline b2Vec2 convert2Meters(const float factor, const utils::Vector2 &input)
    {
        return b2Vec2(factor * input.getX(), factor * input.getY());
    }

    inline utils::Vector2 convertToPixels(const float factor, const b2Vec2 &input)
    {
        return utils::Vector2(factor * input.x, factor * input.y);
    }

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
            bool existsScriptA = entityA.HasComponent<core::ecs::ScriptComponentList>();
            bool existsScriptB = entityB.HasComponent<core::ecs::ScriptComponentList>();

            if (existsScriptA)
            {
                auto &scriptList = entityA.findComponent<core::ecs::ScriptComponentList>();
                for (auto &script : scriptList.components)
                {
                    script.Instance->beginCollision(core::ecs::Collision{entityB});
                }
            }
            if (existsScriptB)
            {
                auto &scriptList = entityB.findComponent<core::ecs::ScriptComponentList>();
                for (auto &script : scriptList.components)
                {
                    script.Instance->beginCollision(core::ecs::Collision{entityA});
                }
            }
        }

        /// Called when two fixtures cease to touch.
        virtual void EndContact(b2Contact *contact) override
        {
            core::ecs::Entity entityA = {(entt::entity)contact->GetFixtureA()->GetBody()->GetUserData().pointer, scene};
            core::ecs::Entity entityB = {(entt::entity)contact->GetFixtureB()->GetBody()->GetUserData().pointer, scene};
            bool existsScriptA = entityA.HasComponent<core::ecs::ScriptComponentList>();
            bool existsScriptB = entityB.HasComponent<core::ecs::ScriptComponentList>();

            if (existsScriptA)
            {
                auto &scriptList = entityA.findComponent<core::ecs::ScriptComponentList>();
                for (auto &script : scriptList.components)
                {
                    script.Instance->endCollision(core::ecs::Collision{entityB});
                }
            }
            if (existsScriptB)
            {
                auto &scriptList = entityB.findComponent<core::ecs::ScriptComponentList>();
                for (auto &script : scriptList.components)
                {
                    script.Instance->endCollision(core::ecs::Collision{entityA});
                }
            }
        }
    };

    class RayCastCallback : public b2RayCastCallback
    {
        Scene *scene;
        std::vector<RayCastHit> results;

    public:
        RayCastCallback(Scene *scene) : scene(scene) {}
        float ReportFixture(b2Fixture *fixture,
                            [[maybe_unused]] const b2Vec2 &point,
                            [[maybe_unused]] const b2Vec2 &normal,
                            [[maybe_unused]] float fraction)
        {
            uintptr_t userData = fixture->GetBody()->GetUserData().pointer;
            if (fixture->GetFilterData().categoryBits == FC_ENTITY)
            {
                core::ecs::Entity entity = {(entt::entity)userData, scene};
                results.push_back(RayCastHit(entity, std::nullopt, convertToPixels(scene->pixelPerMeter, point)));
                return 1;
            }
            else if (fixture->GetFilterData().categoryBits == FC_STATIC_COLLIDER && userData)
            {
                size_t blockPosition = static_cast<size_t>(userData);
                for (auto &block : scene->m_staticBlockCollider)
                {
                    if (block.blockId == blockPosition)
                    {
                        results.push_back(RayCastHit(std::nullopt, block, convertToPixels(scene->pixelPerMeter, point)));
                        break;
                    }
                }
            }
            return 1;
        }

        std::vector<RayCastHit> &getResults()
        {
            return results;
        }
    };

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
        m_entitiesAdded.push_back(handle);
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
            auto &cameraViewPort = renderer->getMainCamera()->getViewPortRect();

            auto group = m_registry.view<core::ecs::Transform>();
            renderer->setDrawColor(0, 0, 255, 255);
            graphics::Rect displayRect;
            for (auto &e : group)
            {

                auto transform = group.get<core::ecs::Transform>(e);
                transform.displayRect(displayRect);
                if (!displayRect.intersects(cameraViewPort))
                    continue;
                transform.position -= cameraPosition;
                if (m_registry.any_of<core::ecs::RenderComponent>(e))
                {
                    auto &rendererComponent = m_registry.get<core::ecs::RenderComponent>(e);
                    rendererComponent.render(transform, renderer);
                }
                else if (m_registry.any_of<core::ecs::TextureMapAnimationRenderComponent>(e))
                {
                    auto &rendererComponent = m_registry.get<core::ecs::TextureMapAnimationRenderComponent>(e);
                    graphics::Rect r{transform.position.getX(), transform.position.getY(), float(transform.width), float(transform.height)};
                    rendererComponent.animation.render(renderer, r);
                }
                if (m_physicsDebug)
                {
                    transform.displayRect(displayRect);
                    renderer->drawRect(displayRect);
                }
            }
        }

        // debug physics renderer
        if (m_physicsDebug)
        {
            renderer->setDrawColor(255, 0, 0, 255);
            auto body = m_PhysicsWorld->GetBodyList();

            while (body != nullptr)
            {

                utils::Vector2 pos = convertToPixels(pixelPerMeter, body->GetTransform().p);
                SDL_FPoint points[5];
                for (b2Fixture *fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
                {
                    if (fixture->GetShape()->GetType() == b2Shape::e_polygon)
                    {

                        b2PolygonShape *poligonShape = (b2PolygonShape *)fixture->GetShape();

                        for (int32 i = 0; i < poligonShape->m_count; i++)
                        {

                            utils::Vector2 p1 = convertToPixels(pixelPerMeter, poligonShape->m_vertices[i]);

                            points[i].x = p1.getX() + pos.getX() - cameraPosition.getX();
                            points[i].y = p1.getY() + pos.getY() - cameraPosition.getY();
                        }

                        utils::Vector2 p1 = convertToPixels(pixelPerMeter, poligonShape->m_vertices[0]);

                        points[4].x = p1.getX() + pos.getX() - cameraPosition.getX();
                        points[4].y = p1.getY() + pos.getY() - cameraPosition.getY();

                        if (SDL_RenderDrawLinesF(renderer->getRenderer(), points, 5))
                        {
                            throw SDLException("SDL_RenderDrawLines");
                        }
                    }
                }
                body = body->GetNext();
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
            boxShape.SetAsBox(bc2d.Size.getX() * transform.Scale.getX() / 2.0, bc2d.Size.getY() * transform.Scale.getY() / 2.0, {bc2d.Offset.getX(), bc2d.Offset.getY()}, 0);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.density = bc2d.Density;
            fixtureDef.friction = bc2d.Friction;
            fixtureDef.restitution = bc2d.Restitution;
            fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
            fixtureDef.filter.categoryBits = FC_ENTITY;
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
            fixtureDef.filter.categoryBits = FC_ENTITY;
            body->CreateFixture(&fixtureDef);
        }
    }

    void Scene::addStaticBlockCollider(std::vector<graphics::Rect> collider)
    {
        for (auto &c : collider)
        {
            auto block = lastBlockId++;
            m_staticBlockCollider.push_back(StaticCollisionBlock{.rect = c, .blockId = block});
        }
    }
    void Scene::addStaticBlockCollider(std::vector<StaticCollisionBlock> collider)
    {
        for (auto &c : collider)
        {
            auto block = lastBlockId++;
            c.blockId = block;
            m_staticBlockCollider.push_back(c);
        }
    }

    void Scene::removeStaticBlockCollider(const std::string &blockData)
    {
        m_staticBlockColliderToDestroy.push_back(blockData);
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

        for (size_t i = 0; i < m_staticBlockCollider.size(); ++i)
        {
            auto &collider = m_staticBlockCollider[i];
            initPhysicsForStaticCollider(collider, collider.blockId);
        }
    }

    void Scene::initPhysicsForStaticCollider(StaticCollisionBlock &collider, size_t blockIndex)
    {
        b2PolygonShape boxShape;
        boxShape.SetAsBox(0.5, 0.5, {0.5, 0.5}, 0);

        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = b2Vec2(collider.rect.x, collider.rect.y);
        bodyDef.angle = 0;

        b2Body *body = m_PhysicsWorld->CreateBody(&bodyDef);
        body->SetFixedRotation(false);
        body->GetUserData().pointer = blockIndex;

        collider.physicsBody = body;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 0;
        fixtureDef.restitutionThreshold = 0;
        fixtureDef.filter.categoryBits = FC_STATIC_COLLIDER;
        body->CreateFixture(&fixtureDef);
    }

    void Scene::fixedUpdateEntities(uint32_t delta)
    {

        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        m_PhysicsWorld->Step(delta / 1000.f, velocityIterations, positionIterations);

        // Retrieve transform from Box2D
        auto view = m_registry.view<core::ecs::Rigidbody2DComponent, core::ecs::Transform>();
        for (auto [e, rb2d, transform] : view.each())
        {

            core::ecs::Entity entity = {e, this};

            b2Body *body = (b2Body *)rb2d.RuntimeBody;
            if (!body)
                continue;
            transform.position = convertToPixels(pixelPerMeter, body->GetPosition());
            // transform.Rotation.z = body->GetAngle();
            if (m_registry.any_of<core::ecs::ScriptComponentList>(e))
            {
                auto &scriptList = m_registry.get<core::ecs::ScriptComponentList>(e);
                for (auto &script : scriptList.components)
                {
                    script.Instance->onUpdate(delta);
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
        m_registry.sort<core::ecs::Transform>([](const auto &lhs, const auto &rhs)
                                              { 
                                                if(lhs.position.getY() + lhs.height == rhs.position.getY() + rhs.height)
                                                    return lhs.position.getX() + lhs.width < rhs.position.getX() + rhs.width;
                                                return lhs.position.getY() + lhs.height < rhs.position.getY() + rhs.height; });
    }

    bool Scene::handleEventsEntities(core::Input *input)
    {
        auto view = m_registry.view<core::ecs::ScriptComponentList, core::ecs::Transform>();
        bool result = false;
        graphics::Rect displayRect;
        auto &cameraViewPort = renderer->getMainCamera()->getViewPortRect();

        for (auto &e : view)
        {
            if (!result)
            {

                auto transform = view.get<core::ecs::Transform>(e);
                transform.displayRect(displayRect);
                if (!displayRect.intersects(cameraViewPort))
                    continue;

                auto &scriptList = view.get<core::ecs::ScriptComponentList>(e);
                for (auto &script : scriptList.components)
                {
                    result = script.Instance->onHandleInput(input);
                }
            }
        }
        return result;
    }

    Scene::~Scene()
    {
        auto view = m_registry.view<core::ecs::ScriptComponentList, core::ecs::Transform>();

        for (auto &e : view)
        {
            if (m_registry.any_of<core::ecs::Rigidbody2DComponent>(e))
            {
                auto &rb2d = m_registry.get<core::ecs::Rigidbody2DComponent>(e);
                m_PhysicsWorld->DestroyBody((b2Body *)rb2d.RuntimeBody);
            }
            if (m_registry.any_of<core::ecs::ScriptComponentList>(e))
            {
                auto compList = m_registry.get<core::ecs::ScriptComponentList>(e);
                for (auto &c : compList.components)
                {
                    c.DestroyScript(&c);
                }
                compList.components.clear();
            }
            m_registry.destroy(e);
        }
        for (auto it = m_staticBlockCollider.begin(); it != m_staticBlockCollider.end(); ++it)
        {

            if (it->physicsBody != nullptr)
                m_PhysicsWorld->DestroyBody((b2Body *)it->physicsBody);
            it->physicsBody = nullptr;
        }

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

        for (auto &handle : m_entitiesAdded)
        {
            if (m_registry.any_of<core::ecs::ScriptComponent>(handle))
            {
                auto &script = m_registry.get<core::ecs::ScriptComponent>(handle);
                if (script.Instance == nullptr)
                {
                    core::ecs::Entity entity = {handle, this};
                    script.Instance = script.InstantiateScript();
                    script.Instance->setEntity(entity);
                }
            }

            if (m_registry.any_of<core::ecs::Rigidbody2DComponent>(handle))
            {
                if (m_registry.get<core::ecs::Rigidbody2DComponent>(handle).RuntimeBody == nullptr)
                    initPhysicsForEntity(handle);
            }
        }

        for (auto &e : m_entitiesToDestroy)
        {
            if (m_registry.any_of<core::ecs::Rigidbody2DComponent>(e))
            {
                auto &rb2d = m_registry.get<core::ecs::Rigidbody2DComponent>(e);
                m_PhysicsWorld->DestroyBody((b2Body *)rb2d.RuntimeBody);
            }
            if (m_registry.any_of<core::ecs::ScriptComponentList>(e))
            {
                auto compList = m_registry.get<core::ecs::ScriptComponentList>(e);
                for (auto &c : compList.components)
                {
                    c.DestroyScript(&c);
                }
                compList.components.clear();
            }
            m_registry.destroy(e);
        }

        for (auto &blockData : m_staticBlockColliderToDestroy)
        {
            for (auto it = m_staticBlockCollider.begin(); it != m_staticBlockCollider.end(); ++it)
            {
                if (it->blockData == blockData)
                {

                    if (it->physicsBody != nullptr)
                        m_PhysicsWorld->DestroyBody((b2Body *)it->physicsBody);
                    it->physicsBody = nullptr;
                    m_staticBlockCollider.erase(it);
                    break;
                }
            }
        }

        if (!m_entitiesToDestroy.empty())
            m_entitiesToDestroy.clear();
        if (!m_entitiesAdded.empty())
            m_entitiesAdded.clear();
        if (!m_staticBlockColliderToDestroy.empty())
            m_staticBlockColliderToDestroy.clear();
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
    void Scene::setPhysicsDebug(bool debug)
    {
        m_physicsDebug = debug;
    }

    bool Scene::getPhysicsDebug()
    {
        return m_physicsDebug;
    }

    std::shared_ptr<UI::WindowManager> &Scene::getWindowManager()
    {
        return winMgr;
    }

    core::RayCastResult Scene::raycast(const utils::Vector2 &startPosition, const utils::Vector2 &endPosition)
    {
        b2Vec2 startPoint = convert2Meters(metersPerPixel, startPosition);
        b2Vec2 endPoint = convert2Meters(metersPerPixel, endPosition);
        auto raycastCallback = std::make_unique<RayCastCallback>(this);

        m_PhysicsWorld->RayCast(raycastCallback.get(), startPoint, endPoint);

        return core::RayCastResult{.hits = raycastCallback->getResults()};
    }

} /* namespace character */
