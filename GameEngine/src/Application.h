#pragma once
#include <memory>
#include <string>

namespace GameEngine {

	class Window;
	class Renderer;
	class Event;

	class Application {
	public:
		Application(const std::string& name);
		~Application();

		void run();
		
	private:
		void eventHandler(Event* event);

	private:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
		bool m_running = true;
	};

}