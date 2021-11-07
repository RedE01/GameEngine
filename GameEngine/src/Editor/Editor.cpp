#include "Editor.h"
#include "../Entity.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include <cassert>

namespace GameEngine {

	Editor::Editor(Vector2 viewportSize) {
		Entity cameraEntity = Entity(&m_editorEntityRegistry, m_editorEntityRegistry.create());
		cameraEntity.addComponent<TransformComponent>();
		cameraEntity.addComponent<CameraComponent>(ProjectionType::Perspective, glm::radians(45.0f), viewportSize);
	}

	Editor::~Editor() {

	}

	void Editor::update() {
		auto cameraView = m_editorEntityRegistry.view<CameraComponent>();
		for(auto entity : cameraView) {
			CameraComponent& cameraComponent = cameraView.get<CameraComponent>(entity);
			cameraComponent.updateCamera();
		}
	}

	Camera* Editor::getEditorCamera() {
		auto cameraView = m_editorEntityRegistry.view<CameraComponent>();

		assert(!cameraView.empty());
		return &(cameraView.get<CameraComponent>(cameraView[0]).getCamera());
	}

	void Editor::updateEditorCameraViewportSize(Vector2 viewportSize) {
		getEditorCamera()->setSize(viewportSize);
	}


}