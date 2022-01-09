#pragma once
#include "../Rendering/Model.h"
#include "AssetHandle.h"
#include "AssetData.h"

namespace GameEngine {

	class AssetManager;

	struct ModelLoader {
        using IDtype = AssetHandle<Model>::IDtype;
		std::shared_ptr<Model> load(AssetData<Model>* assetData, AssetManager* assetManager) const;
	};

}
