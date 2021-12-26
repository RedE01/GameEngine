#pragma once
#include "../../Components/Component.h"

namespace GameEngine {

	class CameraController : public Component {
	public:
        virtual std::string getName() const override { return "Camera Controller"; }
		virtual void update() override;
	};

}
