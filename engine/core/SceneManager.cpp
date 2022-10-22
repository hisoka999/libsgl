/*
 * SceneManager.cpp
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#include "engine/core/SceneManager.h"
#include "engine/core/Scene.h"

namespace core
{

    SceneManager::SceneManager() : nextScene(nullptr)
    {
        // TODO Auto-generated constructor stub
    }

    SceneManager::~SceneManager()
    {
        scenes.clear();
    }

    void SceneManager::render()
    {
        if (currentScene.empty())
            return;
        scenes[currentScene]->render();
    }
    bool SceneManager::handleEvents(core::Input *pInput)
    {
        if (currentScene.empty())
            return false;
        return scenes[currentScene]->handleEvents(pInput);
    }

    void SceneManager::update()
    {
        if (currentScene.empty())
            return;
        scenes[currentScene]->update();
    }

    void SceneManager::addScene(std::string name, const std::shared_ptr<Scene> &scene)
    {

        scenes[name] = scene;
    }

    void SceneManager::setCurrentScene(std::string name)
    {
        if (scenes.count(name) == 0)
        {
            throw std::runtime_error(
                "scene with name \"" + name + "\" does not exists.");
        }
        if (currentScene.size() != 0)
        {
            auto &scene = scenes[currentScene];
            scene->unload();
        }

        currentScene = name;
        scenes[currentScene]->load();
    }

    const std::shared_ptr<Scene> &SceneManager::getScene(std::string name)
    {
        return scenes[name];
    }
    Scene *SceneManager::getCurrentScene()
    {
        return scenes[currentScene].get();
    }

    void SceneManager::fixedUpdate(uint32_t delta)
    {
        if (currentScene.empty())
            return;
        if (nextScene != nullptr)
        {
            currentScene = nextSceneName;
            addScene(currentScene, nextScene);
            nextScene = nullptr;
            nextSceneName = "";
        }

        scenes[currentScene]->fixedUpdate(delta);
    }

    void SceneManager::changeScene(const std::string &name, const std::shared_ptr<Scene> &scene)
    {
        nextSceneName = name;
        nextScene = scene;
    }

    void SceneManager::freeScenes()
    {
        scenes.clear();
    }

} /* namespace core */
