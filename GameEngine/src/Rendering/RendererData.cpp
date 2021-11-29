#include "RendererData.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Texture.h"
#include "Renderbuffer.h"
#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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

	const char* postProcessingVertexShader = (
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

	const char* geometryPassVertexShader = (
		"#version 430 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"layout (location = 2) in vec2 aTextureCoords;\n"
		"\n"
		"out vec3 fragPos;\n"
		"out vec3 normal;\n"
		"out vec2 textureCoords;\n"
		"\n"
		"uniform mat4 u_modelMatrix;\n"
		"uniform mat4 u_viewMatrix;\n"
		"uniform mat4 u_projectionMatrix;\n"
		"\n"
		"void main() {\n"
		"    vec4 fragPosVec4 = u_modelMatrix * vec4(aPos, 1.0);\n"
		"    fragPos = fragPosVec4.xyz;\n"
		"    normal = mat3(transpose(inverse(u_modelMatrix))) * aNormal;\n"
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
		"in vec3 normal;\n"
		"in vec2 textureCoords;\n"
		"uniform sampler2D u_diffuseTexture;\n"
		"\n"
		"void main() {\n"
		"	gPosition = fragPos;\n"
		"	gNormal = normal;\n"
		"	gAlbedo = texture(u_diffuseTexture, textureCoords).xyz;\n"
		"}\n"
	);

	RendererData::RendererData() {
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
		m_postProcessingShader = std::make_unique<Shader>(postProcessingVertexShader, postProcessingFragmentShader);
		m_postProcessingShader->useShader();
		m_postProcessingShader->setUniform1i("u_frameTexture", 0);

		m_postProcessingFramebuffer = std::make_unique<Framebuffer>();
		m_postProcessingFramebuffer->bind();

		m_postProcessingTexture = std::make_unique<Texture>();
		m_postProcessingTexture->bind();
		m_postProcessingTexture->textureImage2D(TextureFormat::RGBA16F, 640, 480, (unsigned char*)NULL);
		m_postProcessingFramebuffer->attachTexture(m_postProcessingTexture.get(), FramebufferAttachmentType::Color0);

		m_postProcessingDepthStencilRenderbuffer = std::make_unique<Renderbuffer>();
		m_postProcessingDepthStencilRenderbuffer->bind();
		m_postProcessingDepthStencilRenderbuffer->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, 640, 480);
		m_postProcessingFramebuffer->attachRenderbuffer(m_postProcessingDepthStencilRenderbuffer.get(), FramebufferAttachmentType::DepthStencil);

		if(!m_postProcessingFramebuffer->isComplete()) std::cout << "FRAMEBUFFER NOT COMPLETE!!" << std::endl;


		// Initialize gBuffer stuff
		m_gBufferShader = std::make_unique<Shader>(geometryPassVertexShader, geometryShaderFragmentShader);

		m_gBufferFramebuffer = std::make_unique<Framebuffer>();
		m_gBufferFramebuffer->bind();

		m_gBufferPosition = std::make_unique<Texture>();
		m_gBufferPosition->textureImage2D(TextureFormat::RGBA16F, 640, 480, (float*)NULL);
		m_gBufferFramebuffer->attachTexture(m_gBufferPosition.get(), FramebufferAttachmentType::Color0);

		m_gBufferNormal = std::make_unique<Texture>();
		m_gBufferNormal->textureImage2D(TextureFormat::RGBA16F, 640, 480, (float*)NULL);
		m_gBufferFramebuffer->attachTexture(m_gBufferNormal.get(), FramebufferAttachmentType::Color1);

		m_gBufferAlbedo = std::make_unique<Texture>();
		m_gBufferAlbedo->textureImage2D(TextureFormat::RGBA, 640, 480, (unsigned char*)NULL);
		m_gBufferFramebuffer->attachTexture(m_gBufferAlbedo.get(), FramebufferAttachmentType::Color2);

		unsigned int drawbuffers[] = {0, 1, 2};
		m_gBufferFramebuffer->setDrawBuffers(drawbuffers, 3);

		m_gBufferDepthStencil = std::make_unique<Renderbuffer>();
		m_gBufferDepthStencil->bind();
		m_gBufferDepthStencil->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, 640, 480);
		m_gBufferFramebuffer->attachRenderbuffer(m_gBufferDepthStencil.get(), FramebufferAttachmentType::DepthStencil);

		if(!m_gBufferFramebuffer->isComplete()) std::cout << "FRAMEBUFFER NOT COMPLETE!!" << std::endl;
	}

	RendererData::~RendererData() {
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

	Texture* RendererData::getPostProcessingTexture() const {
		return m_postProcessingTexture.get();
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

}