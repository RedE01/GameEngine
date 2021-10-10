#pragma once
#include "Math/Vector.h"
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
		Vector2i getWindowSize() const { return m_data.windowSize; }
		Vector2i getWindowPos() const { return m_data.windowPos; }

		GLFWwindow* getWindow() const; 
		void setEventFunction(std::function<void(Event*)> eventFunction);
		
	private:
		void setEventCallbackFunctions();

	private:
		GLFWwindow* m_window;

		struct windowData {
			std::string title;
			Vector2i windowSize;
			Vector2i windowPos;
			std::function<void(Event*)> eventFunction;
		};

		windowData m_data;
	};
}