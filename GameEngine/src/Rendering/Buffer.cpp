#include "Buffer.h"
#include <GL/glew.h>

namespace GameEngine {

    GLenum getGLUsage(BufferDataUsage usage);

    Buffer::Buffer() {
        glGenBuffers(1, &m_bufferID);
    }

    Buffer::~Buffer() {
        glDeleteBuffers(1, &m_bufferID);
    }

    void Buffer::setData(void* data, unsigned int dataSize, BufferDataUsage usageType) {
        bind();

        GLenum glUsage = getGLUsage(usageType);

        glBufferData(getBufferType(), dataSize, data, glUsage);

        m_dataSize = dataSize;
    }

    void Buffer::bind() {
        glBindBuffer(getBufferType(), m_bufferID);
    }

    void Buffer::unbind() {
        glBindBuffer(getBufferType(), 0);
    }

    GLenum getGLUsage(BufferDataUsage usage) {
        switch(usage) {
            case BufferDataUsage::STREAM_DRAW: GL_STREAM_DRAW;
            case BufferDataUsage::STREAM_READ: GL_STREAM_READ;
            case BufferDataUsage::STREAM_COPY: GL_STREAM_COPY;
            case BufferDataUsage::STATIC_DRAW: GL_STATIC_DRAW;
            case BufferDataUsage::STATIC_READ: GL_STATIC_READ;
            case BufferDataUsage::STATIC_COPY: GL_STATIC_COPY;
            case BufferDataUsage::DYNAMIC_DRAW: GL_DYNAMIC_DRAW;
            case BufferDataUsage::DYNAMIC_READ: GL_DYNAMIC_READ;
            case BufferDataUsage::DYNAMIC_COPY: GL_DYNAMIC_COPY;
        }
        return 0;
    }

}