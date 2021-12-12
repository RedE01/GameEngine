#pragma once
#include "../Assets/AssetHandle.h"

namespace GameEngine {

	class Material {
	public:
		ShaderAsset shader;
		TextureAsset diffuseTexture;
        TextureAsset normalTexture;
	};

}
