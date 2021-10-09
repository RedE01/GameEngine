#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GameEngine {

	Application::Application(const std::string& name) {
		GLFWwindow* window;

		if (!glfwInit()) return;

		window = glfwCreateWindow(640, 480, name.c_str(), NULL, NULL);
		if (!window) {
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window);

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);


			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return;	
	}

	void Application::run() {
		// while(true) {

		// }
	}

}