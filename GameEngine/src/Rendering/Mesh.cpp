#include "Mesh.h"
#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace GameEngine {

	Mesh::Mesh(const std::vector<Vertex>& vertexData, const std::vector<unsigned int>& indexData) {
		m_vao = std::make_unique<VertexArrayObject>();
		m_vao->bind();

		std::vector<VertexAttribute> vertexAttributes = {VertexAttribute(3, VertexAttributeType::FLOAT, false), VertexAttribute(3, VertexAttributeType::FLOAT, false), VertexAttribute(2, VertexAttributeType::FLOAT, false)};
		m_vbo = std::make_unique<VertexBuffer>(vertexAttributes);
		m_vbo->bind();
		m_vbo->setData((void*)vertexData.data(), vertexData.size() * sizeof(Vertex), BufferDataUsage::STATIC_DRAW);

		m_ibo = std::make_unique<IndexBuffer>();
		m_ibo->bind();
		m_ibo->setData((void*)indexData.data(), indexData.size() * sizeof(unsigned int), BufferDataUsage::STATIC_DRAW);

		m_vao->unbind();
	}

	Mesh::~Mesh() {
		m_vbo->unbind();
		m_ibo->unbind();
		m_vao->unbind();
	}

	void Mesh::bind() {
		m_vao->bind();
	}

	unsigned int Mesh::getVertexDataSize() const {
		if(m_vbo.get()) return m_vbo->getDataSize();
		return 0;
	}

	unsigned int Mesh::getIndexCount() const {
		if(m_ibo.get()) return m_ibo->getIndexCount();
		return 0;
	}

}