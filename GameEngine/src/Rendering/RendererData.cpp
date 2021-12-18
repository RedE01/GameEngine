#include "RendererData.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Texture.h"
#include "Renderbuffer.h"
#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "../assets/models/sphere.h"

namespace GameEngine {

	float frameVBOdata[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};

	unsigned int frameIBOdata[] = {
		0, 1, 2, 2, 3, 0
	};

	const char* renderToTextureVertexShader = (
		"#version 430 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"layout (location = 1) in vec2 aTextureCoords;\n"
		"\n"
		"out vec2 textureCoords;\n"
		"\n"
		"void main() {\n"
		"    textureCoords = aTextureCoords;\n"
		"    gl_Position = vec4(aPos, 0.0, 1.0);\n"
		"}\n"
	);

	const char* postProcessingFragmentShader = (
		"#version 430 core\n"
		"\n"
		"out vec4 FragColor;\n"
		"in vec2 textureCoords;\n"
		"uniform sampler2D u_frameTexture;\n"
		"\n"
		"void main() {\n"
		"    float gamma = 2.2;\n"
        "    vec3 fragColor = texture(u_frameTexture, textureCoords).rgb;"
		"    FragColor = vec4(pow(fragColor, vec3(1.0 / gamma)), 1.0);\n"
		"}\n"
	);

	const char* finalRenderFragmentShader = (
		"#version 430 core\n"
		"\n"
		"out vec4 FragColor;\n"
		"in vec2 textureCoords;\n"
		"uniform sampler2D u_frameTexture;\n"
		"\n"
		"void main() {\n"
        "    vec3 fragColor = texture(u_frameTexture, textureCoords).rgb;"
		"    FragColor = vec4(fragColor, 1.0);\n"
		"}\n"
	);

	const char* geometryPassVertexShader = (
		"#version 430 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"layout (location = 2) in vec2 aTextureCoords;\n"
		"layout (location = 3) in vec3 aTangent;\n"
		"\n"
		"out vec3 fragPos;\n"
        "out mat3 TBN;\n"
		"out vec2 textureCoords;\n"
		"\n"
		"uniform mat4 u_modelMatrix;\n"
		"uniform mat4 u_viewMatrix;\n"
		"uniform mat4 u_projectionMatrix;\n"
		"\n"
		"void main() {\n"
		"    vec4 fragPosVec4 = u_modelMatrix * vec4(aPos, 1.0);\n"
		"    fragPos = fragPosVec4.xyz;\n"
        "    \n"
        "    mat3 toWordSpace = mat3(transpose(inverse(u_modelMatrix)));\n"
        "    vec3 T = normalize(vec3(toWordSpace * aTangent));\n"
        "    vec3 N = normalize(vec3(toWordSpace * aNormal));\n"
        "    vec3 B = cross(N, T);\n"
        "    TBN = mat3(T, B, N);\n"
        "    \n"
		"    textureCoords = aTextureCoords;\n"
		"    \n"
		"    gl_Position = u_projectionMatrix * u_viewMatrix * fragPosVec4;\n"
		"}\n"
	);

	const char* geometryShaderFragmentShader = (
		"#version 430 core\n"
		"\n"
		"layout (location = 0) out vec3 gPosition;\n"
		"layout (location = 1) out vec3 gNormal;\n"
		"layout (location = 2) out vec3 gAlbedo;\n"
		"\n"
		"in vec3 fragPos;\n"
		"in mat3 TBN;\n"
		"in vec2 textureCoords;\n"
		"uniform sampler2D u_diffuseTexture;\n"
		"uniform sampler2D u_normalTexture;\n"
		"\n"
		"void main() {\n"
		"	gPosition = fragPos;\n"
        "   \n"
		"	gNormal = texture(u_normalTexture, textureCoords).xyz;\n"
		"	gNormal = gNormal * 2.0 - 1;\n"
		"	gNormal = normalize(TBN * gNormal);\n"
        "   \n"
		"	vec4 albedo = texture(u_diffuseTexture, textureCoords).rgba;\n"
        "   gAlbedo = albedo.rgb;\n"
        "   if(albedo.a < 0.1) discard;\n"
		"}\n"
	);

	RendererData::RendererData(Vector2i viewportSize) {
		// Initialize render quad
		m_renderQuadVAO = std::make_unique<VertexArrayObject>();
		m_renderQuadVAO->bind();
		std::vector<VertexAttribute> framebufferAtributes = {VertexAttribute(2, VertexAttributeType::FLOAT, false), VertexAttribute(2, VertexAttributeType::FLOAT, false)};
		m_renderQuadVBO = std::make_unique<VertexBuffer>(framebufferAtributes);
		m_renderQuadVBO->bind();
		m_renderQuadVBO->setData(frameVBOdata, sizeof(frameVBOdata), BufferDataUsage::STATIC_DRAW);
		m_renderQuadIBO = std::make_unique<IndexBuffer>();
		m_renderQuadIBO->bind();
		m_renderQuadIBO->setData(frameIBOdata, sizeof(frameIBOdata), BufferDataUsage::STATIC_DRAW);
		m_renderQuadVAO->unbind();


		// Initialize post processing stuff
		m_postProcessingShader = std::make_unique<Shader>(renderToTextureVertexShader, postProcessingFragmentShader);
		m_postProcessingShader->useShader();
		m_postProcessingShader->setUniform1i("u_frameTexture", 0);

		m_postProcessingFramebuffer = std::make_unique<Framebuffer>();
		m_postProcessingFramebuffer->bind();

		m_frameTexture = std::make_unique<Texture>();
		m_frameTexture->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (unsigned char*)NULL);
		m_postProcessingFramebuffer->attachTexture(m_frameTexture.get(), FramebufferAttachmentType::Color0);

		m_postProcessingDepthStencilRenderbuffer = std::make_unique<Renderbuffer>();
		m_postProcessingDepthStencilRenderbuffer->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, viewportSize.x, viewportSize.y);
		m_postProcessingFramebuffer->attachRenderbuffer(m_postProcessingDepthStencilRenderbuffer.get(), FramebufferAttachmentType::DepthStencil);

		if(!m_postProcessingFramebuffer->isComplete()) std::cout << "FRAMEBUFFER NOT COMPLETE!!" << std::endl;


		// Initialize gBuffer stuff
		m_gBufferShader = std::make_unique<Shader>(geometryPassVertexShader, geometryShaderFragmentShader);

		m_gBufferFramebuffer = std::make_unique<Framebuffer>();
		m_gBufferFramebuffer->bind();

		m_gBufferPosition = std::make_unique<Texture>();
		m_gBufferPosition->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (float*)NULL);
		m_gBufferFramebuffer->attachTexture(m_gBufferPosition.get(), FramebufferAttachmentType::Color0);

		m_gBufferNormal = std::make_unique<Texture>();
		m_gBufferNormal->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (float*)NULL);
		m_gBufferFramebuffer->attachTexture(m_gBufferNormal.get(), FramebufferAttachmentType::Color1);

		m_gBufferAlbedo = std::make_unique<Texture>();
		m_gBufferAlbedo->textureImage2D(TextureFormat::RGBA, viewportSize.x, viewportSize.y, (unsigned char*)NULL);
		m_gBufferFramebuffer->attachTexture(m_gBufferAlbedo.get(), FramebufferAttachmentType::Color2);

		unsigned int drawbuffers[] = {0, 1, 2};
		m_gBufferFramebuffer->setDrawBuffers(drawbuffers, 3);

		m_gBufferDepthStencil = std::make_unique<Renderbuffer>();
        m_gBufferDepthStencil->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, viewportSize.x, viewportSize.y);
		m_gBufferFramebuffer->attachRenderbuffer(m_gBufferDepthStencil.get(), FramebufferAttachmentType::DepthStencil);

		if(!m_gBufferFramebuffer->isComplete()) std::cout << "FRAMEBUFFER NOT COMPLETE!!" << std::endl;

		// Initialize lighting-pass stuff
		m_lightingFramebuffer = std::make_unique<Framebuffer>();
		m_lightingFramebuffer->bind();

		m_lightingTexture = std::make_unique<Texture>();
		m_lightingTexture->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (unsigned char*)NULL);
		m_lightingFramebuffer->attachTexture(m_lightingTexture.get(), FramebufferAttachmentType::Color0);

		m_lightingDepthStencil = std::make_unique<Renderbuffer>();
		m_lightingDepthStencil->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, viewportSize.x, viewportSize.y);
		m_lightingFramebuffer->attachRenderbuffer(m_lightingDepthStencil.get(), FramebufferAttachmentType::DepthStencil);

		if(!m_lightingFramebuffer->isComplete()) std::cout << "FRAMEBUFEFR NOT COMPLETE!" << std::endl;

        m_finalRenderShader = std::make_unique<Shader>(renderToTextureVertexShader, finalRenderFragmentShader);

		// Initialize sphere
        m_sphereVAO = std::make_unique<VertexArrayObject>();
        m_sphereVAO->bind();
        std::vector<VertexAttribute> sphereAttributes = {VertexAttribute(3, VertexAttributeType::FLOAT, false)};
        m_sphereVBO = std::make_unique<VertexBuffer>(sphereAttributes);
        m_sphereVBO->bind();
        m_sphereVBO->setData(sphereVertexData, sizeof(sphereVertexData), BufferDataUsage::STATIC_DRAW);
        m_sphereIBO = std::make_unique<IndexBuffer>();
        m_sphereIBO->bind();
        m_sphereIBO->setData(sphereIndexData, sizeof(sphereIndexData), BufferDataUsage::STATIC_DRAW);
        m_sphereVAO->unbind();

	}

	RendererData::~RendererData() {
	}

	void RendererData::setDefaultShader(ShaderAsset defaultShader) {
		m_lightingShader = defaultShader;
	}

    void RendererData::setViewportSize(Vector2i viewportSize) {
		m_postProcessingDepthStencilRenderbuffer->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, viewportSize.x, viewportSize.y);
		m_frameTexture->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (unsigned char*)NULL);

        m_gBufferDepthStencil->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, viewportSize.x, viewportSize.y);
		m_gBufferAlbedo->textureImage2D(TextureFormat::RGBA, viewportSize.x, viewportSize.y, (unsigned char*)NULL);
		m_gBufferNormal->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (float*)NULL);
		m_gBufferPosition->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (float*)NULL);

		m_lightingDepthStencil->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, viewportSize.x, viewportSize.y);
		m_lightingTexture->textureImage2D(TextureFormat::RGBA16F, viewportSize.x, viewportSize.y, (unsigned char*)NULL);
    }

	VertexArrayObject* RendererData::getRenderQuadVAO() const {
		return m_renderQuadVAO.get();
	}

	unsigned int RendererData::getRenderQuadIndexCount() const {
		return m_renderQuadIBO->getIndexCount();
	}

	Shader* RendererData::getPostProcessingShader() const {
		return m_postProcessingShader.get();
	}

	Framebuffer* RendererData::getPostProcessingFramebuffer() const {
		return m_postProcessingFramebuffer.get();
	}

    std::shared_ptr<Texture> RendererData::getFrameTexture() const {
		return m_frameTexture;
	}

	Shader* RendererData::getGeometryPassShader() const {
		return m_gBufferShader.get();
	}

	Framebuffer* RendererData::getGBufferFramebuffer() const {
		return m_gBufferFramebuffer.get();
	}

	Texture* RendererData::getGBufferPosition() const {
		return m_gBufferPosition.get();
	}

	Texture* RendererData::getGBufferNormal() const {
		return m_gBufferNormal.get();
	}

	Texture* RendererData::getGBufferAlbedo() const {
		return m_gBufferAlbedo.get();
	}

	ShaderAsset RendererData::getDefaultLightingShader() const {
		return m_lightingShader;
	}

	Framebuffer* RendererData::getLightingFramebuffer() const {
		return m_lightingFramebuffer.get();
	}

	Texture* RendererData::getLightingTexture() const {
		return m_lightingTexture.get();
	}

    Shader* RendererData::getFinalRenderShader() const {
        return m_finalRenderShader.get();
    }

    VertexArrayObject* RendererData::getSphereVAO() const {
        return m_sphereVAO.get();
    }

    unsigned int RendererData::getSphereIndexCount() const {
        return m_sphereIBO->getIndexCount();
    }

}
