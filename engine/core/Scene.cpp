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

    void Scene::fixedUpdate(u_int32_t delta)
    {
    }

    Scene::~Scene()
    {
        winMgr = nullptr;
        music = nullptr;
        // TODO Auto-generated destructor stub
    }

} /* namespace character */
