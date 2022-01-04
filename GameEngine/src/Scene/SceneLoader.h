#pragma once
#include <memory>

namespace GameEngine {

    class Scene;
    class AssetManager;

	struct SceneLoader {
		std::shared_ptr<Scene> load(const std::string& filepath, AssetManager* assetManager) const;
		void serialize(const std::string& filepath, const std::string& sceneName, Scene* scene, AssetManager* assetManager) const;
	};

}
