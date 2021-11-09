#include "Editor.h"
#include "../Entity.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "Components/CameraController.h"
#include <cassert>

namespace GameEngine {

	Editor::Editor(Vector2 viewportSize) {
		Entity cameraEntity = Entity(&m_editorEntityRegistry, m_editorEntityRegistry.create());
		cameraEntity.addComponent<TransformComponent>();
		cameraEntity.addComponent<CameraComponent>(ProjectionType::Perspective, glm::radians(45.0f), viewportSize);
		cameraEntity.addComponent<CameraController>();
	}

	Editor::~Editor() {

	}

	void Editor::update() {
		m_editorEntityRegistry.view<CameraController>().each([](CameraController& cameraController){
			cameraController.update();
		});

		m_editorEntityRegistry.view<CameraComponent>().each([](CameraComponent& cameraComponent){
			cameraComponent.updateCamera();
		});
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