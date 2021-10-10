#pragma once
#include <memory>
#include <string>

namespace GameEngine {

	class Window;
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
		bool m_running = true;
	};

}