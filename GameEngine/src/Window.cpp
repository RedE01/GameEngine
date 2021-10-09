#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

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
		m_data.width = width;
		m_data.height = height;

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);

		glfwSwapInterval(true);
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

}