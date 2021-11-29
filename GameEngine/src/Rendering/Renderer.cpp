#include "Renderer.h"
#include "../Components/MeshRendererComponent.h"
#include "../Components/TransformComponent.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderbuffer.h"
#include "RendererData.h"
#include "Camera.h"
#include "Debug.h"
#include <GL/glew.h>

#include <gtc/matrix_transform.hpp>

namespace GameEngine {

	Renderer::Renderer(Vector2 viewportSize) {
		if(glewInit() != GLEW_OK) return;

		initializeRendererDebugger();

		m_rendererData = std::make_unique<RendererData>();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);

		setViewportSize(viewportSize);
	}

	Renderer::~Renderer() {
	}

	void Renderer::beginFrame() {
		m_rendererData->getGBufferFramebuffer()->bind();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::endFrame() {
		// Lighing pass
		m_rendererData->getLightingFramebuffer()->bind();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		m_rendererData->getRenderQuadVAO()->bind();

		m_rendererData->getDefaultLightingShader()->useShader();
		m_rendererData->getDefaultLightingShader()->setUniform1i("u_gPosition", 0);
		m_rendererData->getDefaultLightingShader()->setUniform1i("u_gNormal", 1);
		m_rendererData->getDefaultLightingShader()->setUniform1i("u_gAlbedo", 2);
		glActiveTexture(GL_TEXTURE0);
		m_rendererData->getGBufferPosition()->bind();
		glActiveTexture(GL_TEXTURE1);
		m_rendererData->getGBufferNormal()->bind();
		glActiveTexture(GL_TEXTURE2);
		m_rendererData->getGBufferAlbedo()->bind();

		glDrawElements(GL_TRIANGLES, m_rendererData->getRenderQuadIndexCount(), GL_UNSIGNED_INT, 0);


		// Post processing
		m_rendererData->getLightingFramebuffer()->unbind();
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		m_rendererData->getPostProcessingShader()->useShader();
		glActiveTexture(GL_TEXTURE0);
		m_rendererData->getLightingTexture()->bind();
		m_rendererData->getPostProcessingShader()->setUniform1i("u_frameTexture", 0);

		glDrawElements(GL_TRIANGLES, m_rendererData->getRenderQuadIndexCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::renderEntities(entt::registry& entityRegistry, Camera* camera) {
		if(camera == nullptr) return;

		auto meshRendererView = entityRegistry.view<MeshRendererComponent>();
		for(auto& entity : meshRendererView) {
			auto& meshRendererComponent = meshRendererView.get<MeshRendererComponent>(entity);
			auto& transformComponent = entityRegistry.get<TransformComponent>(entity);

			if(meshRendererComponent.model) {
				renderModel(&meshRendererComponent.model.get(), &transformComponent, camera);
			}
		}
	}

	void Renderer::setViewportSize(Vector2 viewportSize) {
		glViewport(0, 0, viewportSize.x, viewportSize.y);
	}

	void Renderer::setDefaultShader(ShaderAsset shaderAsset) {
		m_rendererData->setDefaultShader(shaderAsset);
	}

	void Renderer::renderModel(Model* model, TransformComponent* transform, Camera* camera) {
		for(auto& mesh : model->meshes) {
			if(mesh && mesh->material && mesh->material->shader) {
				m_rendererData->getGeometryPassShader()->useShader();

				glActiveTexture(GL_TEXTURE0);
				if(mesh->material->texture) {
					mesh->material->texture->bind();
				}

				Matrix4 modelMat = transform->getMatrix();

				m_rendererData->getGeometryPassShader()->setUniformMat4("u_modelMatrix", modelMat);
				m_rendererData->getGeometryPassShader()->setUniformMat4("u_viewMatrix", camera->getViewMatrix());
				m_rendererData->getGeometryPassShader()->setUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());

				mesh->bind();

				glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
			}
		}
	}


}