#include "Application.h"
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Components/ScriptComponentManager.h"
#include "Events/Event.h"

namespace GameEngine {

	Application::Application(const std::string& name) {
		m_window = std::make_unique<Window>(name, 640, 480);
		m_renderer = std::make_unique<Renderer>();
		m_scene = std::make_unique<Scene>();
		m_scriptComponentManager = std::make_unique<ScriptComponentManager>();

		m_window->setEventFunction(std::bind(&Application::eventHandler, this, std::placeholders::_1));
	}

	Application::~Application() {

	}

	void Application::run() {
		registerScriptComponents(m_scriptComponentManager.get());

		while(m_running) {
			Input::Update(m_window->getWindowSize());

			m_scene->update(m_scriptComponentManager.get());

			m_renderer->renderFrame();

			m_window->pollEvents();
			m_window->swapBuffers();
		}
	}

	Scene* Application::getScene() const {
		return m_scene.get();
	}

	void Application::eventHandler(Event* event) {
		if(event->isInCategory(EventCategory::Input)) {
			Input::HandleEvent(event);
		}
		else if(event->isInCategory(EventCategory::Application)) {
			switch(event->getEventType()) {
			case EventType::WindowClose: m_running = false; break;
			}
		}
	}

}