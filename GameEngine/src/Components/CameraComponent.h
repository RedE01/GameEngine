#pragma once
#include "Component.h"
#include "../Rendering/Camera.h"

namespace GameEngine {

	class CameraComponent : public Component {
	public:
        CameraComponent();
		CameraComponent(ProjectionType type, float fov, Vector2 size);
        virtual std::string getName() const override { return GetName(); }
        static std::string GetName() { return "Camera"; }

		void updateCamera();

		const Camera& getCamera() const;
		Camera& getCamera();

	private:
		Camera m_camera;
	};

}
