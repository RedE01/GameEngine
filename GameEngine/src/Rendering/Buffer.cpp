#include "Buffer.h"
#include <assert.h>
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
            case BufferDataUsage::STREAM_DRAW: return GL_STREAM_DRAW;
            case BufferDataUsage::STREAM_READ: return GL_STREAM_READ;
            case BufferDataUsage::STREAM_COPY: return GL_STREAM_COPY;
            case BufferDataUsage::STATIC_DRAW: return GL_STATIC_DRAW;
            case BufferDataUsage::STATIC_READ: return GL_STATIC_READ;
            case BufferDataUsage::STATIC_COPY: return GL_STATIC_COPY;
            case BufferDataUsage::DYNAMIC_DRAW: return GL_DYNAMIC_DRAW;
            case BufferDataUsage::DYNAMIC_READ: return GL_DYNAMIC_READ;
            case BufferDataUsage::DYNAMIC_COPY: return GL_DYNAMIC_COPY;
        }
        assert(0);
        return 0;
    }

}