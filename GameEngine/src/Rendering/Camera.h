#pragma once
#include "../Math/Matrix.h"
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace GameEngine {

	enum class ProjectionType {
		Perspective, Orthographic
	};

	class Camera {
	public:
		Camera(ProjectionType type, float fov, Vector2 size);

		ProjectionType getProjectionType() const;
		float getFov() const;
		Vector2 getSize() const;
		float getNearPlane() const;
		float getFarPlane() const;

		void setProjectionType(ProjectionType projectionType);
		void setFov(float fov);
		void setSize(Vector2 size);
		void setNearPlane(float nearPlane);
		void setFarPlane(float farPlane);

		const Matrix4& getProjectionMatrix() const;
		Matrix4 getViewMatrix() const;

	private:
		void recalculateProjectionMatrix();

	public:
		bool isActive = true;
		Vector3 position;
		Quaternion orientation;

	private:
		ProjectionType m_projectionType = ProjectionType::Perspective;
		float m_fov = glm::radians(45.0f);
		Vector2 m_size = Vector2(100.0, 100.0);

		float m_nearPlane = 0.1f;
		float m_farPlane = 100.0f;

		Matrix4 m_projectionMatrix;
	};

}