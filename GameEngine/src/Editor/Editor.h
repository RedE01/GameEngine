#pragma once
#include "../Math/Vector.h"
#include <entt/entity/registry.hpp>

namespace GameEngine {

	class Camera;

	class Editor {
	public:
		Editor(Vector2 viewportSize);
		~Editor();

		void update();

		Camera* getEditorCamera();
		void updateEditorCameraViewportSize(Vector2 viewportSize);

	private:
		entt::registry m_editorEntityRegistry;
	};

}