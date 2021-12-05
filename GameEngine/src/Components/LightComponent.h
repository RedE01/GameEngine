#pragma once
#include "Component.h"
#include "../Assets/AssetHandle.h"

namespace GameEngine {

	class LightComponent : public Component {
	public:
        float attenuationConstant = 1.0f;
        float attenuationLinear = 0.7f;
        float attenuationQuadratic = 1.8f;
	};

}

