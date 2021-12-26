#include "Editor.h"
#include "../Entity.h"
#include "../Window.h"
#include "../Application.h"
#include "ViewportWindow.h"
#include "SceneWindow.h"
#include "PropertiesWindow.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "Components/CameraController.h"
#include <cassert>

#include <iostream>

namespace GameEngine {

	Editor::Editor(Application* application) : m_application(application), m_editorGui(application, this) {
		Entity cameraEntity = Entity(&m_editorEntityRegistry, m_editorEntityRegistry.create());
		cameraEntity.addComponent<TransformComponent>();
		cameraEntity.addComponent<CameraComponent>(ProjectionType::Perspective, glm::radians(45.0f), m_application->getWindow()->getWindowSize());
		cameraEntity.addComponent<CameraController>();

        m_editorGui.addWindow<ViewportWindow>();
        m_editorGui.addWindow<SceneWindow>();
        m_editorGui.addWindow<PropertiesWindow>();
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

    void Editor::render() {
        m_editorGui.OnGuiRender();
    }

	Camera* Editor::getEditorCamera() {
		auto cameraView = m_editorEntityRegistry.view<CameraComponent>();

		assert(!cameraView.empty());
		return &(cameraView.get<CameraComponent>(cameraView[0]).getCamera());
	}

    Entity Editor::getSelectedEntity() {
        return m_selectedEntity;
    }

    void Editor::setEventFunction(std::function<void(Event*)> eventFunction) {
        m_editorGui.setEventFunction(eventFunction);
    }

    void Editor::setViewportTexture(std::shared_ptr<Texture> viewportTexture) {
        std::vector<ViewportWindow*> viewportWindows = m_editorGui.getWindows<ViewportWindow>();
        for(auto viewportWindow : viewportWindows) {
            viewportWindow->setTexture(viewportTexture);
        }
    }

	void Editor::updateEditorCameraViewportSize(Vector2 viewportSize) {
		getEditorCamera()->setSize(viewportSize);
	}


    void Editor::selectEntity(Entity entity) {
        m_selectedEntity = entity;
    }

}
