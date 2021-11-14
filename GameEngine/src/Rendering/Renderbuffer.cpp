#include "Renderbuffer.h"
#include <GL/glew.h>

namespace GameEngine {

	Renderbuffer::Renderbuffer() {
		glGenRenderbuffers(1, &m_renderbufferID);
	}

	Renderbuffer::~Renderbuffer() {
		glDeleteRenderbuffers(1, &m_renderbufferID);
	}

	void Renderbuffer::bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferID);
	}

	void Renderbuffer::createRenderbufferStorage(TextureFormat format, unsigned int width, unsigned int height) {
		internal::OpenGLTextureFormats openglformats = internal::getOpenGLTextureFormats(format);
		glRenderbufferStorage(GL_RENDERBUFFER, openglformats.internalFormat, width, height);
	}

	unsigned int Renderbuffer::getID() const {
		return m_renderbufferID;
	}

}