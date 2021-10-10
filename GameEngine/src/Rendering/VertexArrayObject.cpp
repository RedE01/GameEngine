#include "VertexArrayObject.h"
#include <GL/glew.h>

namespace GameEngine {

    VertexArrayObject::VertexArrayObject() {
        glGenVertexArrays(1, &m_vao);
        bind();
    }
    VertexArrayObject::~VertexArrayObject() {
        glDeleteVertexArrays(1, &m_vao);
    }

    void VertexArrayObject::bind() {
        glBindVertexArray(m_vao);
    }
    void VertexArrayObject::unbind() {
        glBindVertexArray(0);
    }

}