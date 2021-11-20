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
		"    FragColor = textureCoords.x * texture(u_frameTexture, textureCoords);\n"
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

}