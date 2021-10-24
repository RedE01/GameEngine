#pragma once
#include "../Math/Vector.h"
#include <vector>
#include <memory>

namespace GameEngine {

	class VertexArrayObject;
	class VertexBuffer;
	class IndexBuffer;
	class Material;

	struct Vertex {
		Vector3 position;
		Vector3 normal;
		Vector2 textureCoords;
	};

	class Mesh {
	public:
		Mesh(const std::vector<Vertex>& vertexData, const std::vector<unsigned int>& indexData);
		Mesh(const Mesh& mesh) = delete;
		Mesh(Mesh&& mesh) = delete;
		~Mesh();

		void bind();
		unsigned int getVertexDataSize() const;
		unsigned int getIndexCount() const;

	private:
		std::unique_ptr<VertexArrayObject> m_vao;
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<IndexBuffer> m_ibo;
		std::shared_ptr<Material> m_material;
	};
}