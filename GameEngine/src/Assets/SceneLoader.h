#pragma once
#include "../Scene.h"
#include <memory>

namespace GameEngine {

	struct SceneLoader {
		std::shared_ptr<Scene> load(const std::string& filepath) const;
		void serialize(const std::string& filepath, const std::string& sceneName, Scene* scene) const;
	};

}
