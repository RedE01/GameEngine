#pragma once
#include <vector>

namespace GameEngine {

    class VertexArrayObject {
    public:
        VertexArrayObject();
        ~VertexArrayObject();

        void bind();
        void unbind();

        unsigned int getVertexArrayObjectID() const { return m_vao; }

    private:
        unsigned int m_vao;
    };

}