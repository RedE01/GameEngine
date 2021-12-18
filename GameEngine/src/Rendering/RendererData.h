#pragma once
#include "../Math/Vector.h"
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
		RendererData(Vector2i viewportSize);
		~RendererData();

		void setDefaultShader(ShaderAsset defaultShader);
        void setViewportSize(Vector2i viewportSize);

		VertexArrayObject* getRenderQuadVAO() const;
		unsigned int getRenderQuadIndexCount() const;

		Shader* getPostProcessingShader() const;
		Framebuffer* getPostProcessingFramebuffer() const;
        std::shared_ptr<Texture> getFrameTexture() const;

		Shader* getGeometryPassShader() const;
		Framebuffer* getGBufferFramebuffer() const;
		Texture* getGBufferPosition() const;
		Texture* getGBufferNormal() const;
		Texture* getGBufferAlbedo() const;

		ShaderAsset getDefaultLightingShader() const;
		Framebuffer* getLightingFramebuffer() const;
		Texture* getLightingTexture() const;

        Shader* getFinalRenderShader() const;

		VertexArrayObject* getSphereVAO() const;
		unsigned int getSphereIndexCount() const;

	private:
		std::unique_ptr<VertexArrayObject> m_renderQuadVAO;
		std::unique_ptr<VertexBuffer> m_renderQuadVBO;
		std::unique_ptr<IndexBuffer> m_renderQuadIBO;

		std::unique_ptr<Shader> m_postProcessingShader;
		std::unique_ptr<Framebuffer> m_postProcessingFramebuffer;
		std::shared_ptr<Texture> m_frameTexture;
		std::unique_ptr<Renderbuffer> m_postProcessingDepthStencilRenderbuffer;

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

		std::unique_ptr<Shader> m_finalRenderShader;

		std::unique_ptr<VertexArrayObject> m_sphereVAO;
		std::unique_ptr<VertexBuffer> m_sphereVBO;
		std::unique_ptr<IndexBuffer> m_sphereIBO;
	};

}
