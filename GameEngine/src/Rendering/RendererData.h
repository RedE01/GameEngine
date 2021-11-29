#pragma once
#include "../Assets/AssetHandle.h"
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

		void setDefaultShader(ShaderAsset defaultShader);

		VertexArrayObject* getRenderQuadVAO() const;
		unsigned int getRenderQuadIndexCount() const;

		Shader* getPostProcessingShader() const;
		Framebuffer* getPostProcessingFramebuffer() const;
		Texture* getPostProcessingTexture() const;

		Shader* getGeometryPassShader() const;
		Framebuffer* getGBufferFramebuffer() const;
		Texture* getGBufferPosition() const;
		Texture* getGBufferNormal() const;
		Texture* getGBufferAlbedo() const;

		ShaderAsset getDefaultLightingShader() const;
		Framebuffer* getLightingFramebuffer() const;
		Texture* getLightingTexture() const;

	private:
		std::unique_ptr<VertexBuffer> m_renderQuadVBO;
		std::unique_ptr<IndexBuffer> m_renderQuadIBO;

		std::unique_ptr<Shader> m_postProcessingShader;
		std::unique_ptr<Framebuffer> m_postProcessingFramebuffer;
		std::unique_ptr<Texture> m_postProcessingTexture;
		std::unique_ptr<Renderbuffer> m_postProcessingDepthStencilRenderbuffer;
		std::unique_ptr<VertexArrayObject> m_renderQuadVAO;

		std::unique_ptr<Shader> m_gBufferShader;
		std::unique_ptr<Framebuffer> m_gBufferFramebuffer;
		std::unique_ptr<Texture> m_gBufferPosition;
		std::unique_ptr<Texture> m_gBufferNormal;
		std::unique_ptr<Texture> m_gBufferAlbedo;
		std::unique_ptr<Renderbuffer> m_gBufferDepthStencil;

		ShaderAsset m_lightingShader;
		std::unique_ptr<Framebuffer> m_lightingFramebuffer;
		std::unique_ptr<Texture> m_lightingTexture;
		std::unique_ptr<Renderbuffer> m_lightingDepthStencil;
	};

}