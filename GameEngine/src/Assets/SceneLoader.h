#pragma once
#include "../Scene.h"
#include <memory>

namespace GameEngine {

    class AssetManager;

	struct SceneLoader {
		std::shared_ptr<Scene> load(const std::string& filepath, AssetManager* assetManager) const;
		void serialize(const std::string& filepath, const std::string& sceneName, Scene* scene, AssetManager* assetManager) const;
	};

}
