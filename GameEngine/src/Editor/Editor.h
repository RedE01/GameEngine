#pragma once
#include "../Math/Vector.h"
#include "Gui.h"
#include <entt/entity/registry.hpp>

namespace GameEngine {

	class Camera;
    class Texture;

	class Editor {
	public:
		Editor(Window* window);
		~Editor();

		void update();
        void render();

		Camera* getEditorCamera();
        void setViewportTexture(std::shared_ptr<Texture> viewportTexture);
		void updateEditorCameraViewportSize(Vector2 viewportSize);

	private:
		entt::registry m_editorEntityRegistry;
        Gui m_editorGui;
	};

}
