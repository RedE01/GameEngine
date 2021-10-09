#pragma once
#include <string>
#include <functional>

struct GLFWwindow;

namespace GameEngine {

	class Window {
	public:
		Window(const std::string& title, unsigned int width, unsigned int height);
		~Window();

		void swapBuffers();
		void pollEvents();

		GLFWwindow* getWindow() const; 

	private:
		GLFWwindow* m_window;

		struct windowData {
			std::string title;
			unsigned int width, height;
		};

		windowData m_data;
	};
}