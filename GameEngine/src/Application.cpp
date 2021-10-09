#include "Application.h"
#include "Window.h"

namespace GameEngine {

	Application::Application(const std::string& name) {
		m_window = std::make_unique<Window>(name, 640, 480);
	}

	Application::~Application() {

	}

	void Application::run() {
		while(true) {
			m_window->pollEvents();
			m_window->swapBuffers();
		}
	}

}