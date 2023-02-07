#pragma once
#include <engine/utils/vector2.h>
#include <engine/core/renderer.h>
#include <engine/graphics/texture.h>
#include <functional>
#include <memory>
#include "Entity.h"
#include <engine/graphics/TextureMapAnimation.h>
#include <cassert>
#include <engine/physics/PhysicsBody.h>

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

    struct RectangleCollider
    {
        physics::PhysicsBody body;
    };

    struct BoxListCollider
    {
        std::vector<physics::PhysicsBody> bodies;
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
        virtual void onUpdate(size_t delta) = 0;
        virtual bool onHandleInput([[maybe_unused]] core::Input *input)
        {
            return false;
        }
        virtual void onCollision([[maybe_unused]] ScriptableEntity *entity) {}
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