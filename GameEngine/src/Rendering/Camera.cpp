#include "Camera.h"

#include <gtc/matrix_transform.hpp>

namespace GameEngine {

	Camera::Camera(ProjectionType type, float fov, Vector2 size) 
		: position(Vector3(0.0)), orientation(1.0, 0.0, 0.0, 0.0), m_projectionType(type), m_fov(fov), m_size(size) {

		recalculateProjectionMatrix();
	}

	ProjectionType Camera::getProjectionType() const {
		return m_projectionType;
	}

	float Camera::getFov() const {
		return m_fov;
	}

	Vector2 Camera::getSize() const {
		return m_size;
	}

	float Camera::getNearPlane() const {
		return m_nearPlane;
	}
	
	float Camera::getFarPlane() const {
		return m_farPlane;
	}

	void Camera::setProjectionType(ProjectionType projectionType) {
		m_projectionType = projectionType;
		recalculateProjectionMatrix();
	}

	void Camera::setFov(float fov) {
		m_fov = fov;
		recalculateProjectionMatrix();
	}

	void Camera::setSize(Vector2 size) {
		m_size = size;
		recalculateProjectionMatrix();
	}

	void Camera::setNearPlane(float nearPlane) {
		m_nearPlane = nearPlane;
		recalculateProjectionMatrix();
	}

	void Camera::setFarPlane(float farPlane) {
		m_farPlane = farPlane;
		recalculateProjectionMatrix();
	}

	const Matrix4& Camera::getProjectionMatrix() const {
		return m_projectionMatrix;
	}

	Matrix4 Camera::getViewMatrix() const {
        Matrix4 matrix = glm::toMat4(- orientation);
        matrix = glm::translate(matrix, - position);

        return matrix;
	}

	void Camera::recalculateProjectionMatrix() {
		switch(m_projectionType) {
		case ProjectionType::Perspective: m_projectionMatrix = glm::perspective(m_fov, m_size.x / m_size.y, m_nearPlane, m_farPlane); break;
		case ProjectionType::Orthographic: m_projectionMatrix = glm::ortho(0.0f, m_size.x, 0.0f, m_size.y, m_nearPlane, m_farPlane); break;
		}
	}

}