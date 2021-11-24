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

	const char* frameVertexShader = (
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

	const char* frameFragmentShader = (
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
		m_frameShader = std::make_unique<Shader>(frameVertexShader, frameFragmentShader);
		m_frameShader->useShader();
		m_frameShader->setUniform1i("u_frameTexture", 0);

		m_framebuffer = std::make_unique<Framebuffer>();
		m_framebuffer->bind();

		m_frameTexture = std::make_unique<Texture>();
		m_frameTexture->bind();
		m_frameTexture->textureImage2D(TextureFormat::RGBA16F, 640, 480, (unsigned char*)NULL);
		m_framebuffer->attachTexture(m_frameTexture.get(), FramebufferAttachmentType::Color0);

		m_frameDepthStencilRenderbuffer = std::make_unique<Renderbuffer>();
		m_frameDepthStencilRenderbuffer->bind();
		m_frameDepthStencilRenderbuffer->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, 640, 480);
		m_framebuffer->attachRenderbuffer(m_frameDepthStencilRenderbuffer.get(), FramebufferAttachmentType::DepthStencil);

		if(!m_framebuffer->isComplete()) std::cout << "FRAMEBUFFER NOT COMPLETE!!" << std::endl;

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


		m_geometryPassShader = std::make_unique<Shader>(geometryPassVertexShader, geometryShaderFragmentShader);

		m_gBuffer = std::make_unique<Framebuffer>();
		m_gBuffer->bind();

		m_gBufferPosition = std::make_unique<Texture>();
		m_gBufferPosition->textureImage2D(TextureFormat::RGBA16F, 640, 480, (float*)NULL);
		m_gBuffer->attachTexture(m_gBufferPosition.get(), FramebufferAttachmentType::Color0);

		m_gBufferNormal = std::make_unique<Texture>();
		m_gBufferNormal->textureImage2D(TextureFormat::RGBA16F, 640, 480, (float*)NULL);
		m_gBuffer->attachTexture(m_gBufferNormal.get(), FramebufferAttachmentType::Color1);

		m_gBufferAlbedo = std::make_unique<Texture>();
		m_gBufferAlbedo->textureImage2D(TextureFormat::RGBA, 640, 480, (unsigned char*)NULL);
		m_gBuffer->attachTexture(m_gBufferAlbedo.get(), FramebufferAttachmentType::Color2);

		unsigned int drawbuffers[] = {0, 1, 2};
		m_gBuffer->setDrawBuffers(drawbuffers, 3);

		m_gBufferDepthStencil = std::make_unique<Renderbuffer>();
		m_gBufferDepthStencil->bind();
		m_gBufferDepthStencil->createRenderbufferStorage(TextureFormat::DEPTH_STENCIL, 640, 480);
		m_gBuffer->attachRenderbuffer(m_gBufferDepthStencil.get(), FramebufferAttachmentType::DepthStencil);

		if(!m_gBuffer->isComplete()) std::cout << "FRAMEBUFFER NOT COMPLETE!!" << std::endl;
	}

	RendererData::~RendererData() {
	}

	Framebuffer* RendererData::getFramebuffer() const {
		return m_framebuffer.get();
	}

	Shader* RendererData::getFrameShader() const {
		return m_frameShader.get();
	}

	Texture* RendererData::getFrameTexture() const {
		return m_frameTexture.get();
	}

	VertexArrayObject* RendererData::getRenderQuadVAO() const {
		return m_renderQuadVAO.get();
	}

	unsigned int RendererData::getRenderQuadIndexCount() const {
		return m_renderQuadIBO->getIndexCount();
	}

	Shader* RendererData::getGeometryPassShader() const {
		return m_geometryPassShader.get();
	}

	Framebuffer* RendererData::getGBuffer() const {
		return m_gBuffer.get();
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