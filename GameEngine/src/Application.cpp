#include "Application.h"
#include "Window.h"
#include "Input.h"
#include "Events/Event.h"

namespace GameEngine {

	Application::Application(const std::string& name) {
		m_window = std::make_unique<Window>(name, 640, 480);
		
		m_window->setEventFunction(std::bind(&Application::eventHandler, this, std::placeholders::_1));
	}

	Application::~Application() {

	}

	void Application::run() {
		while(m_running) {
			Input::Update(m_window->getWindowSize());
			m_window->pollEvents();
			m_window->swapBuffers();
		}
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