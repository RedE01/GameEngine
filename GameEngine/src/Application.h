#pragma once
#include <memory>
#include <string>

namespace GameEngine {

	class Window;

	class Application {
	public:
		Application(const std::string& name);
		~Application();

		void run();

	private:
		std::unique_ptr<Window> m_window;
	};

}