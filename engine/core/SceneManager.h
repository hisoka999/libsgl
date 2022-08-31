/*
 * SceneManager.h
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#ifndef CORE_SCENEMANAGER_H_
#define CORE_SCENEMANAGER_H_

#include "engine/core/input.h"
#include <map>
#include <string>
#include <memory>
#include <mutex>

namespace core
{

    class Scene;

    class SceneManager
    {
    public:
        static SceneManager &Instance()
        {
            static SceneManager instance;
            return instance;
        }

        void render();
        bool handleEvents(core::Input *pInput);

        void addScene(std::string name, const std::shared_ptr<Scene> &scene);
        void setCurrentScene(std::string name);
        const std::shared_ptr<Scene> &getScene(std::string name);
        void update();
        const std::shared_ptr<Scene> &getCurrentScene();
        void fixedUpdate(uint32_t delta);
        void changeScene(const std::string &name, const std::shared_ptr<Scene> &scene);
        void freeScenes();

    private:
        SceneManager();
        virtual ~SceneManager();

        std::map<std::string, std::shared_ptr<Scene>> scenes;
        std::string currentScene;
        std::string nextSceneName;
        std::shared_ptr<Scene> nextScene;
    };

} /* namespace core */

#endif /* CORE_SCENEMANAGER_H_ */
