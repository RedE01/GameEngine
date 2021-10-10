#include "IndexBuffer.h"
#include <GL/glew.h>

namespace GameEngine {

    int IndexBuffer::getBufferType() const {
        return GL_ELEMENT_ARRAY_BUFFER;
    }

}