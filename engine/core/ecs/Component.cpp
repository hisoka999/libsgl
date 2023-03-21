#include "Component.h"
#include "box2d/b2_body.h"

namespace core::ecs
{

        void Rigidbody2DComponent::applyLinearImpuls(const utils::Vector2 &impulse, bool wake)
        {
                b2Body *body = (b2Body *)RuntimeBody;
                body->ApplyLinearImpulseToCenter(b2Vec2(impulse.getX(), impulse.getY()), wake);
        }

        void Rigidbody2DComponent::SetLinearVelocity(const utils::Vector2 &vel)
        {
                b2Body *body = (b2Body *)RuntimeBody;
                body->SetLinearVelocity(b2Vec2(vel.getX(), vel.getY()));
        }
}