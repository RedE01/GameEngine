#pragma once
#include "../Rendering/Shader.h"
#include "AssetData.h"
#include <memory>

namespace GameEngine {

	struct ShaderLoader {
		std::shared_ptr<Shader> load(AssetData<Shader>* assetData) const;
	};

}
