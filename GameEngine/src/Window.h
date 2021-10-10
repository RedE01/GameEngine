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
		int getWidth() const { return m_data.width; }
		int getHeight() const { return m_data.height; }

		GLFWwindow* getWindow() const; 
		void setEventFunction(std::function<void(Event*)> eventFunction);
		
	private:
		void setEventCallbackFunctions();

	private:
		GLFWwindow* m_window;

		struct windowData {
			std::string title;
			int width, height;
			std::function<void(Event*)> eventFunction;
		};

		windowData m_data;
	};
}