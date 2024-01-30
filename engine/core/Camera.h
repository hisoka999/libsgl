/*
 * Camera.h
 *
 *  Created on: 20.12.2015
 *      Author: stefan
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <engine/graphics/rect.h>

namespace core
{

    class Camera
    {
    public:
        explicit Camera(graphics::Rect &viewPort);
        virtual ~Camera();

        void move(float pX, float pY);
        void reset();

        [[nodiscard]] float getX() const;
        [[nodiscard]] float getY() const;
        [[nodiscard]] float getWidth() const;
        [[nodiscard]] float getHeight() const;
        utils::Vector2 getPosition() const;

        [[nodiscard]] graphics::Rect &getViewPortRect();

    private:
        graphics::Rect viewPort;
    };

} /* namespace core */

#endif /* CAMERA_H_ */
