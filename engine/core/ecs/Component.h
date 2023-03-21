#pragma once
#include <engine/utils/vector2.h>
#include <engine/core/renderer.h>
#include <engine/graphics/texture.h>
#include <functional>
#include <memory>
#include "Entity.h"
#include <engine/graphics/TextureMapAnimation.h>
#include <cassert>
#include <engine/core/Camera.h>
namespace core
{
    class Input;
}

namespace core::ecs
{

    struct Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;
    };

    struct TagComponent : public Component
    {
        std::string TagName;
        TagComponent(const std::string &TagName) : TagName(TagName)
        {
        }
    };

    struct Transform : Component
    {
        utils::Vector2 position;
        utils::Vector2 Scale = utils::Vector2(1.0f, 1.0f);
        size_t width;
        size_t height;

        void displayRect(graphics::Rect &rect)
        {
            rect.x = position.getX();
            rect.y = position.getY();
            rect.width = float(width);
            rect.height = float(height);
        }
    };

    struct CameraComponent : public Component
    {
        Camera *camera;
        CameraComponent(Camera *camera) : camera(camera){};
    };

    struct Rigidbody2DComponent
    {
        enum class BodyType
        {
            Static = 0,
            Dynamic,
            Kinematic
        };
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        // Storage for runtime
        void *RuntimeBody = nullptr;

        void applyLinearImpuls(const utils::Vector2 &impulse, bool wake);
        void SetLinearVelocity(const utils::Vector2 &vel);

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent &) = default;
    };

    struct BoxCollider2DComponent
    {
        utils::Vector2 Offset = {0.0f, 0.0f};
        utils::Vector2 Size = {0.5f, 0.5f};

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        void *RuntimeFixture = nullptr;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent &) = default;
    };

    struct CircleCollider2DComponent
    {
        utils::Vector2 Offset = {0.0f, 0.0f};
        float Radius = 0.5f;

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        void *RuntimeFixture = nullptr;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent &) = default;
    };

    struct Collision
    {
        Entity entity;
    };

    class ScriptableEntity
    {
    protected:
        Entity entity{};

    public:
        void setEntity(Entity e)
        {
            entity = e;
        }
        Entity &getEntity()
        {
            return entity;
        }
        virtual void onUpdate([[maybe_unused]] size_t delta){};
        virtual bool onHandleInput([[maybe_unused]] core::Input *input)
        {
            return false;
        }
        virtual void onCollision([[maybe_unused]] ScriptableEntity *entity) {}
        virtual void beginCollision([[maybe_unused]] const Collision &collider) {}
        virtual void endCollision([[maybe_unused]] const Collision &collider) {}
        ScriptableEntity() = default;
        virtual ~ScriptableEntity() = default;
    };

    struct ScriptComponent
    {
        ScriptableEntity *Instance = nullptr;

        ScriptableEntity *(*InstantiateScript)();
        void (*DestroyScript)(ScriptComponent *);

        template <typename T>
        void Bind()
        {
            using namespace std::placeholders;
            InstantiateScript = []()
            { return static_cast<ScriptableEntity *>(new T()); };
            DestroyScript = [](ScriptComponent *nsc)
            { delete nsc->Instance; nsc->Instance = nullptr; };
        }
    };

    struct RenderComponent;
    typedef std::function<void(RenderComponent *, graphics::Rect &, core::Renderer *)> RenderFunction;
    struct RenderComponent : Component
    {
    private:
        std::shared_ptr<graphics::Texture> texture;
        std::shared_ptr<graphics::ChildTexture> childTexture;
        RenderFunction renderFunction = nullptr;

    public:
        RenderComponent(RenderFunction renderFunction)
            : texture(nullptr), childTexture(nullptr), renderFunction(renderFunction)
        {
        }

        RenderComponent(std::shared_ptr<graphics::Texture> tex)
            : texture(tex), childTexture(nullptr)
        {
            assert(this->texture);
        }

        RenderComponent(const std::shared_ptr<graphics::ChildTexture> &childTexture)
            : texture(nullptr), childTexture(childTexture)
        {
            assert(this->childTexture);
        }
        void Bind()
        {
            if (texture)
            {
                renderFunction = [](RenderComponent *comp, graphics::Rect &targetRect, core::Renderer *renderer)
                {
                    assert(comp->texture);
                    comp->texture->render(renderer, comp->texture->getTextureRect(), targetRect);
                };
            }
            else if (childTexture)
            {
                renderFunction = [](RenderComponent *comp, graphics::Rect &targetRect, core::Renderer *renderer)
                {
                    assert(comp->childTexture);
                    comp->childTexture->render(targetRect, renderer);
                };
            }
        }

        void render(Transform &transform, core::Renderer *renderer)
        {
            graphics::Rect targetRect;
            transform.displayRect(targetRect);
            if (renderFunction == nullptr)
                Bind();

            renderFunction(this, targetRect, renderer);
        }
    };

    struct TextureMapAnimationRenderComponent : Component
    {

        graphics::TextureMapAnimation animation;

        TextureMapAnimationRenderComponent(graphics::TextureMapAnimation &animation) : animation(animation) {}
        virtual ~TextureMapAnimationRenderComponent() = default;
    };

}