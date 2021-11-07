#pragma once
#include "Component.h"
#include "../Rendering/Camera.h"

namespace GameEngine {

	class CameraComponent : public Component {
	public:
		CameraComponent(ProjectionType type, float fov, Vector2 size);

		void updateCamera();

		const Camera& getCamera() const;
		Camera& getCamera();

	private:
		Camera m_camera;
	};

}