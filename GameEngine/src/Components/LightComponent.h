#pragma once
#include "Component.h"
#include "../Assets/AssetHandle.h"

namespace GameEngine {

    enum class LightType {
        PointLight = 0, DirectionalLight
    };

	class LightComponent : public Component {
	public:
        virtual std::string getName() const override { return "Light"; }
        LightType lightType = LightType::PointLight;
        
        // Point light stuff
        float pointLightRange = 5.0f;

        // Directional light stuff
        float intensity = 0.5f;
	};

}

