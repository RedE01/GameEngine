#pragma once
#include "../Rendering/Model.h"
#include <entt/resource/loader.hpp>

namespace GameEngine {

	class AssetManager;

	struct ModelLoader final : entt::resource_loader<ModelLoader, Model> {
		std::shared_ptr<Model> load(const std::string& filepath, AssetManager* assetManager) const;
	};

}