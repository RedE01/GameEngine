#pragma once
#include "../Rendering/Texture.h"
#include "AssetData.h"
#include <memory>

namespace GameEngine {

	struct TextureLoader {
		std::shared_ptr<Texture> load(AssetData<Texture>* assetData) const;
		std::shared_ptr<Texture> load(AssetData<Texture>* assetData, unsigned char* textureData, unsigned int dataLength) const;
	};

}

