#include "CameraComponent.h"
#include "TransformComponent.h"

namespace GameEngine {

	CameraComponent::CameraComponent(ProjectionType type, float fov, Vector2 size)
		: m_camera(type, fov, size) {

	}

	void CameraComponent::updateCamera() {
		TransformComponent& transform = getEntity().getComponent<TransformComponent>();

		m_camera.position = transform.getPosition();
		m_camera.orientation = transform.getOrientation();
	}

	const Camera& CameraComponent::getCamera() const {
		return m_camera;
	}

	Camera& CameraComponent::getCamera() {
		return m_camera;
	}

}