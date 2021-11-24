#pragma once
#include <memory>

namespace GameEngine {

	class Shader;
	class Framebuffer;
	class Texture;
	class Renderbuffer;
	class VertexArrayObject;
	class VertexBuffer;
	class IndexBuffer;

	class RendererData {
	public:
		RendererData();
		~RendererData();

		Framebuffer* getFramebuffer() const;
		Shader* getFrameShader() const;
		Texture* getFrameTexture() const;
		VertexArrayObject* getRenderQuadVAO() const;
		unsigned int getRenderQuadIndexCount() const;

		Shader* getGeometryPassShader() const;
		Framebuffer* getGBuffer() const;
		Texture* getGBufferPosition() const;
		Texture* getGBufferNormal() const;
		Texture* getGBufferAlbedo() const;

	private:
		std::unique_ptr<Shader> m_frameShader;
		std::unique_ptr<Texture> m_frameTexture;
		std::unique_ptr<Renderbuffer> m_frameDepthStencilRenderbuffer;
		std::unique_ptr<Framebuffer> m_framebuffer;
		std::unique_ptr<VertexArrayObject> m_renderQuadVAO;
		std::unique_ptr<VertexBuffer> m_renderQuadVBO;
		std::unique_ptr<IndexBuffer> m_renderQuadIBO;

		std::unique_ptr<Shader> m_geometryPassShader;
		std::unique_ptr<Framebuffer> m_gBuffer;
		std::unique_ptr<Texture> m_gBufferPosition;
		std::unique_ptr<Texture> m_gBufferNormal;
		std::unique_ptr<Texture> m_gBufferAlbedo;
		std::unique_ptr<Renderbuffer> m_gBufferDepthStencil;
	};

}