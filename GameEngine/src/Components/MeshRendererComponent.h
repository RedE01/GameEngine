#pragma once
#include "Component.h"
#include "../Assets/AssetHandle.h"

namespace GameEngine {

	class MeshRendererComponent : public Component {
	public:
        MeshRendererComponent() {
            registerPublicVariable("model", &MeshRendererComponent::model);
        }

        virtual std::string getName() const override { return GetName(); }
        static std::string GetName() { return "Mesh Renderer"; }
		ModelAsset model;
	};

}
