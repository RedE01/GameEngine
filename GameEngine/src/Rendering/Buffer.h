#pragma once

namespace GameEngine {

    enum class BufferDataUsage {
        STREAM_DRAW, STREAM_READ, STREAM_COPY, STATIC_DRAW, STATIC_READ, STATIC_COPY, DYNAMIC_DRAW, DYNAMIC_READ, DYNAMIC_COPY
    };

    class Buffer {
    public:
        Buffer();
        ~Buffer();
        Buffer(const Buffer& other) = delete;
        Buffer(Buffer&& other) = delete;

        virtual void setData(void* data, unsigned int dataSize, BufferDataUsage usageType);

        virtual void bind();
        virtual void unbind();

        unsigned int getBufferID() const { return m_bufferID; }
        unsigned int getDataSize() const { return m_dataSize; }

        Buffer& operator=(const Buffer& other) = delete;
        Buffer& operator=(Buffer&& other) = delete;

    private:
        virtual unsigned int getBufferType() const = 0;

    private:
        unsigned int m_bufferID;
        unsigned int m_dataSize;
    };

}