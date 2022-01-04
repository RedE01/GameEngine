#pragma once
#include <memory>

namespace GameEngine {

    class Scene;
    class AssetManager;

    class SceneManager {
    public:
        SceneManager(std::shared_ptr<AssetManager> assetManager);
        ~SceneManager();

        bool loadScene(const std::string& filepath);
        void saveActiveScene();
        void saveActiveScene(const std::string& name, const std::string& filepath);
        Scene* getActiveScene() const;

    private:
        void setSceneData(std::shared_ptr<Scene> scene, const std::string& filepath);

    private:
        std::shared_ptr<AssetManager> m_assetManager;
        std::shared_ptr<Scene> m_activeScene;
        std::string m_activeSceneFilepath;
    };

}
