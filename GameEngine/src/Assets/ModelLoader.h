#pragma once
#include "../Rendering/Model.h"

namespace GameEngine {

	class AssetManager;

	struct ModelLoader {
		std::shared_ptr<Model> load(const std::string& filepath, AssetManager* assetManager) const;
	};

}
