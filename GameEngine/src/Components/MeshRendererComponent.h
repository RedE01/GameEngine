#pragma once
#include "Component.h"
#include "../Assets/AssetHandle.h"

namespace GameEngine {

	class MeshRendererComponent : public Component {
	public:
        virtual std::string getName() const override { return "Mesh Renderer"; }
		ModelAsset model;
	};

}
