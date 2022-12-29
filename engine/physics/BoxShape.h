#pragma once

namespace physics
{
    struct BoxShape
    {
        float width;
        float height;
        float mass;
        float momentOfInertia;

        BoxShape(float width, float height, float mass)
            : width(width), height(height), mass(mass)
        {
            momentOfInertia = mass * (width * width + height * height) / 12;
        }
    };

}
