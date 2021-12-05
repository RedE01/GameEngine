#pragma once
#include "Component.h"
#include "../Assets/AssetHandle.h"

namespace GameEngine {

    enum class LightType {
        PointLight = 0, DirectionalLight
    };

	class LightComponent : public Component {
	public:
        LightType lightType = LightType::PointLight;
        
        // Point light stuff
        float attenuationConstant = 1.0f;
        float attenuationLinear = 0.7f;
        float attenuationQuadratic = 1.8f;

        // Directional light stuff
        float intensity = 0.5f;
	};

}

