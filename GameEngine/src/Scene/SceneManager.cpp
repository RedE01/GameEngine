#include "SceneManager.h"
#include "Scene.h"
#include "SceneLoader.h"

namespace GameEngine {

    SceneManager::SceneManager(std::shared_ptr<AssetManager> assetManager) 
        : m_assetManager(assetManager) {
    
        setSceneData(std::make_shared<Scene>("Default Scene"), "");
    }

    SceneManager::~SceneManager() {

    }

    bool SceneManager::loadScene(const std::string& filepath) {
        SceneLoader sceneLoader;
        std::shared_ptr<Scene> newScene = sceneLoader.load(filepath, m_assetManager.get());
        if(newScene) {
            setSceneData(newScene, filepath);
        }
        return true;
    }

    void SceneManager::saveActiveScene() {
        SceneLoader sceneLoader;
        sceneLoader.serialize(m_activeSceneFilepath, getActiveScene()->getName(), getActiveScene(), m_assetManager.get());
    }

    void SceneManager::saveActiveScene(const std::string& name, const std::string& filepath) {
        SceneLoader sceneLoader;
        sceneLoader.serialize(filepath, name, getActiveScene(), m_assetManager.get());
    }

    Scene* SceneManager::getActiveScene() const {
        return m_activeScene.get();
    }

    void SceneManager::setSceneData(std::shared_ptr<Scene> scene, const std::string& filepath) {
        m_activeScene = scene;
        m_activeSceneFilepath = filepath;
    }

}
