#pragma once
#include "Component.h"
#include "../Assets/AssetHandle.h"

namespace GameEngine {

	class MeshRendererComponent : public Component {
	public:
		ModelAsset model;
	};

}