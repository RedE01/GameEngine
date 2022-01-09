#pragma once
#include "AssetData.h"
#include "../Rendering/Material.h"

namespace GameEngine {

    class AssetManager;

	struct MaterialLoader {
		std::shared_ptr<Material> load(AssetData<Material>* assetData, AssetManager* assetManager) const;
        void save(Material* material, const std::string& filepath) const;
	};

}


