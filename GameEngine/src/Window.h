#pragma once
#include <string>
#include <functional>

struct GLFWwindow;

namespace GameEngine {

	class Event;

	class Window {
	public:
		Window(const std::string& title, unsigned int width, unsigned int height);
		~Window();

		void swapBuffers();
		void pollEvents();

		GLFWwindow* getWindow() const; 
		void setEventFunction(std::function<void(Event*)> eventFunction);
		
	private:
		void setEventCallbackFunctions();

	private:
		GLFWwindow* m_window;

		struct windowData {
			std::string title;
			unsigned int width, height;
			std::function<void(Event*)> eventFunction;
		};

		windowData m_data;
	};
}