#pragma once
#include "../Math/Vector.h"
#include "../Assets/AssetHandle.h"
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
        Vector3 tangent;
	};

	class Mesh {
	public:
		Mesh(const std::vector<Vertex>& vertexData, const std::vector<unsigned int>& indexData, MaterialAsset material);
		Mesh(const Mesh& mesh) = delete;
		Mesh(Mesh&& mesh) = delete;
		~Mesh();

		void bind();
		unsigned int getVertexDataSize() const;
		unsigned int getIndexCount() const;

	public:
		MaterialAsset material;

	private:
		std::unique_ptr<VertexArrayObject> m_vao;
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<IndexBuffer> m_ibo;
	};
}
