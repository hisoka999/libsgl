#include "PhysicsBody.h"
#include <iostream>

namespace physics
{
    bool PhysicsBody::checkCollisionDirection(PositionDirection direction)
    {
        for (auto &c : collisions)
        {
            if (c.direction == direction)
            {
                return true;
            }
        }
        return false;
    }

    void PhysicsBody::ComputeTorqueAndForce()
    {
        utils::Vector2 f = utils::Vector2{0, (!checkCollisionDirection(PositionDirection::BottomLeft) && !checkCollisionDirection(PositionDirection::BottomRight)) ? 100.0f : 0.0f};
        force = f;
        // r is the 'arm vector' that goes from the center of mass to the point of force application
        utils::Vector2 r = utils::Vector2{shape->width / 2, shape->height / 2};
        torque = r.getX() * f.getY() - r.getY() * f.getX();
    }

    void PhysicsBody::Run(float dt)
    {
        if (m_bodyType == BodyType::Static)
            return;
        ComputeTorqueAndForce();
        utils::Vector2 linearAcceleration = utils::Vector2{force.getX() / shape->mass, force.getY() / shape->mass};
        linearVelocity += linearAcceleration * dt;
        position += linearVelocity * dt;
        float angularAcceleration = torque / shape->momentOfInertia;
        angularVelocity += angularAcceleration * dt;
        angle += angularVelocity * dt;
    }

    PhysicsBody::PhysicsBody(BodyType bodyType,
                             utils::Vector2 position,
                             utils::Vector2 linearVelocity,
                             float angle,
                             float angularVelocity, BoxShape *shape)
        : m_bodyType(bodyType), linearVelocity(linearVelocity), angle(angle), angularVelocity(angularVelocity), shape(shape), position(position)
    {
    }

    PhysicsBody::~PhysicsBody()
    {
    }

    void PhysicsBody::checkCollision(PhysicsBody *other)
    {
        for (auto it = collisions.begin(); it != collisions.end();)
        {
            if ((*it).other == other)
            {
                it = collisions.erase(it);
            }
            else
            {
                it++;
            }
        }
        auto &pos1 = getPositions();
        auto &pos2 = other->getPositions();
        for (size_t i = 0; i < 4; ++i)
        {
            utils::Vector2 &p1 = pos1[i];
            for (size_t j = 0; j < 4; ++j)
            {
                utils::Vector2 &p2 = pos2[j];
                // std::cout << "(" << p1.getX() << "|" << p1.getY() << ") ---> (" << p2.getX() << "|" << p2.getY() << ") = " << p1.distance(p2) << std::endl;
                float distance = p1.distance(p2);
                if (distance < 5)
                {
                    collisions.push_back({PositionDirection(i), PositionDirection(j), distance, other});
                }
            }
        }
    }

    std::array<utils::Vector2, 4> &PhysicsBody::getPositions()
    {
        if (positions[size_t(PositionDirection::TopLeft)] != position)
        {
            positions[size_t(PositionDirection::TopLeft)] = position;
            positions[size_t(PositionDirection::TopRight)] = {position.getX() + shape->width, position.getY()};
            positions[size_t(PositionDirection::BottomLeft)] = {position.getX(), position.getY() + shape->height};
            positions[size_t(PositionDirection::BottomRight)] = {position.getX() + shape->width, position.getY() + shape->height};
        }
        return positions;
    }
}
