/*
 * Scene.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "engine/core/Scene.h"

namespace core
{

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

    Scene::~Scene()
    {
        winMgr = nullptr;
        music = nullptr;
    }

    void Scene::update()
    {
    }

} /* namespace character */
