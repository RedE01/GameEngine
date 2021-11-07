#include "Application.h"
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Components/ScriptComponentManager.h"
#include "Assets/AssetManager.h"
#include "Events/ApplicationEvent.h"

namespace GameEngine {

	Application::Application(const std::string& name) {
		m_window = std::make_unique<Window>(name, 640, 480);
		m_renderer = std::make_unique<Renderer>();
		m_scene = std::make_unique<Scene>();
		m_scriptComponentManager = std::make_unique<ScriptComponentManager>();
		m_assetManager = std::make_unique<AssetManager>();

		m_window->setEventFunction(std::bind(&Application::eventHandler, this, std::placeholders::_1));
	}

	Application::~Application() {

	}

	void Application::run() {
		registerScriptComponents(m_scriptComponentManager.get());

		GameStartEvent gameStartEvent;
		eventHandler(&gameStartEvent);

		while(m_running) {
			Input::Update(m_window->getWindowSize());

			m_scene->update(m_scriptComponentManager.get());
			onUpdate();

			m_renderer->renderFrame(m_scene.get(), m_scene->getActiveCamera());
			onRender();

			m_window->pollEvents();
			m_window->swapBuffers();
		}

		GameEndEvent gameEndEvent;
		eventHandler(&gameEndEvent);
	}

	Window* Application::getWindow() const {
		return m_window.get();
	}

	Scene* Application::getScene() const {
		return m_scene.get();
	}

	AssetManager* Application::getAssetManager() const {
		return m_assetManager.get();
	}

	void Application::eventHandler(Event* event) {
		if(event->isInCategory(EventCategory::Input)) {
			Input::HandleEvent(event);
		}
		else if(event->isInCategory(EventCategory::Application)) {
			if(event->getEventType() == EventType::WindowClose) {
				m_running = false;
			}
			else if(event->getEventType() == EventType::WindowResize) {
				m_scene->updateCameras(m_window->getWindowSize().x, m_window->getWindowSize().y);
			}
		}
	}

}