/*
 * Camera.cpp
 *
 *  Created on: 20.12.2015
 *      Author: stefan
 */

#include <engine/core/Camera.h>

namespace core
{

        Camera::Camera(graphics::Rect &viewPort)
        {
                this->viewPort = viewPort;
        }

        Camera::~Camera()
        {
        }
        void Camera::reset()
        {
                viewPort.x = 0;
                viewPort.y = 0;
        }
        void Camera::move(float pX, float pY)
        {
                viewPort.x += pX;
                viewPort.y += pY;
        }

        float Camera::getX() const
        {
                return viewPort.x;
        }
        float Camera::getY() const
        {
                return viewPort.y;
        }
        float Camera::getWidth() const
        {
                return viewPort.width;
        }
        float Camera::getHeight() const
        {
                return viewPort.height;
        }

        utils::Vector2 Camera::getPosition()
        {
                return {viewPort.x, viewPort.y};
        }
        const graphics::Rect &Camera::getViewPortRect() const
        {
                return viewPort;
        }
} /* namespace core */
