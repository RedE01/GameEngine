#pragma once
#include "Buffer.h"

namespace GameEngine {

    class IndexBuffer : public Buffer {
    public:
        IndexBuffer() = default;

        unsigned int getIndexCount() const { return getDataSize() / sizeof(unsigned int); }

    private:
        virtual int getBufferType() const override;
    };

}