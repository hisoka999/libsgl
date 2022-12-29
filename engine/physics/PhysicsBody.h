#pragma once
#include "engine/utils/vector2.h"
#include "BoxShape.h"
#include <array>
#include <vector>

namespace physics
{
    enum class BodyType
    {
        Static = 0,
        Dynamic,
        Kinematic
    };
    class PhysicsBody;

    enum class PositionDirection
    {
        TopLeft = 0,
        TopRight = 1,
        BottomLeft = 2,
        BottomRight = 3
    };

    struct Collision
    {
        PositionDirection direction;
        PositionDirection otherDirection;
        float distance;
        PhysicsBody *other;
    };

    class PhysicsBody
    {
    private:
        BodyType m_bodyType;

        utils::Vector2 linearVelocity;
        float angle;
        float angularVelocity;
        utils::Vector2 force;
        float torque;

        bool checkCollisionDirection(PositionDirection direction);
        std::array<utils::Vector2, 4> positions;

    public:
        BoxShape *shape;
        std::vector<Collision> collisions;

        utils::Vector2 position;
        void ComputeTorqueAndForce();
        void Run(float dt);
        PhysicsBody(BodyType bodyType,
                    utils::Vector2 position,
                    utils::Vector2 linearVelocity,
                    float angle,
                    float angularVelocity, BoxShape *shape);
        ~PhysicsBody();
        void checkCollision(PhysicsBody *other);
        std::array<utils::Vector2, 4> &getPositions();
    };

} // namespace physics
