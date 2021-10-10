#include "Renderer.h"
#include <GL/glew.h>

namespace GameEngine {

	Renderer::Renderer() {
		if(glewInit() != GLEW_OK) return;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_FRAMEBUFFER_SRGB); // Temporary
	}

	void Renderer::renderFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


}