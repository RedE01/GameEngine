#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

namespace GameEngine {

	Window::Window(const std::string& title, unsigned int width, unsigned int height) {
		if (!glfwInit()) {
			std::cout << "Could not initialize glfw" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!m_window) {
			glfwTerminate();
			std::cout << "Could not create window" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_data.title = title;
		glfwGetFramebufferSize(m_window, &m_data.windowSize.x, &m_data.windowSize.y);
		glfwGetWindowPos(m_window, &m_data.windowPos.x, &m_data.windowPos.y);

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);

		glfwSwapInterval(true);

		setEventCallbackFunctions();
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::swapBuffers() {
		glfwSwapBuffers(m_window);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

	GLFWwindow* Window::getWindow() const {
		return m_window;
	}

	void Window::setEventFunction(std::function<void(Event*)> eventFunction) {
		m_data.eventFunction = eventFunction;
	}

	void Window::setEventCallbackFunctions() {
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

			if(data.eventFunction) {
				WindowCloseEvent e;
				data.eventFunction(&e);
			}
		});

		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			data.windowSize = Vector2i(width, height);

			if(data.eventFunction) {
				WindowResizeEvent e;
				e.width = width;
				e.height = height;
				data.eventFunction(&e);
			}
		});

		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int x, int y) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			data.windowPos = Vector2i(x, y);

			if(data.eventFunction) {
				WindowMovedEvent e;
				e.xpos = x;
				e.ypos = y;
				data.eventFunction(&e);
			}
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

			if(data.eventFunction) {
				if(action == GLFW_PRESS) {
					KeyPress e(key, scancode, mod);
					data.eventFunction(&e);
				}
				else if(action == GLFW_RELEASE) {
					KeyReleased e(key, scancode, mod);
					data.eventFunction(&e);
				}
				else if(action == GLFW_REPEAT) {
					KeyTyped e(key, scancode, mod);
					data.eventFunction(&e);
				}
			}
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

			if(data.eventFunction) {
				MouseMoved e(xpos, ypos);
				if(data.eventFunction) data.eventFunction(&e);
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

			if(data.eventFunction) {
				if(action == GLFW_PRESS) {
					MouseButtonPressed e(button, mods);
					if(data.eventFunction) data.eventFunction(&e);
				}
				else if(action == GLFW_RELEASE) {
					MouseButtonReleased e(button, mods);
					if(data.eventFunction) data.eventFunction(&e);
				}
			}
		});
	}
}