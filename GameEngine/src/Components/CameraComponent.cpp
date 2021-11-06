#include "CameraComponent.h"
#include "../Entity.h"
#include "TransformComponent.h"

#include <gtc/matrix_transform.hpp>

namespace GameEngine {

	CameraComponent::CameraComponent(ProjectionType type, float fov, Vector2 size) 
		: m_projectionType(type), m_fov(fov), m_size(size) {

		recalculateProjectionMatrix();
	}

	ProjectionType CameraComponent::getProjectionType() const {
		return m_projectionType;
	}

	float CameraComponent::getFov() const {
		return m_fov;
	}

	Vector2 CameraComponent::getSize() const {
		return m_size;
	}

	float CameraComponent::getNearPlane() const {
		return m_nearPlane;
	}
	
	float CameraComponent::getFarPlane() const {
		return m_farPlane;
	}

	void CameraComponent::setProjectionType(ProjectionType projectionType) {
		m_projectionType = projectionType;
		recalculateProjectionMatrix();
	}

	void CameraComponent::setFov(float fov) {
		m_fov = fov;
		recalculateProjectionMatrix();
	}

	void CameraComponent::setSize(Vector2 size) {
		m_size = size;
		recalculateProjectionMatrix();
	}

	void CameraComponent::setNearPlane(float nearPlane) {
		m_nearPlane = nearPlane;
		recalculateProjectionMatrix();
	}

	void CameraComponent::setFarPlane(float farPlane) {
		m_farPlane = farPlane;
		recalculateProjectionMatrix();
	}

	const Matrix4& CameraComponent::getProjectionMatrix() const {
		return m_projectionMatrix;
	}

	Matrix4 CameraComponent::getViewMatrix() const {
		TransformComponent& transform = getEntity().getComponent<TransformComponent>();

        Matrix4 matrix = glm::mat4(1.0);

        matrix = glm::translate(matrix, - transform.getPosition());
        matrix *= glm::toMat4( - transform.getOrientation());

        return matrix;
	}

	void CameraComponent::recalculateProjectionMatrix() {
		switch(m_projectionType) {
		case ProjectionType::Perspective: m_projectionMatrix = glm::perspective(m_fov, m_size.x / m_size.y, m_nearPlane, m_farPlane); break;
		case ProjectionType::Orthographic: m_projectionMatrix = glm::ortho(0.0f, m_size.x, 0.0f, m_size.y, m_nearPlane, m_farPlane); break;
		}
	}

}