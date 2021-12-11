#include "Renderer.h"
#include "../Components/MeshRendererComponent.h"
#include "../Components/LightComponent.h"
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

	Renderer::Renderer(Vector2i viewportSize) : m_viewportSize(viewportSize) {
		if(glewInit() != GLEW_OK) return;

		initializeRendererDebugger();

		m_rendererData = std::make_unique<RendererData>(viewportSize);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		setViewportSize(viewportSize);
	}

	Renderer::~Renderer() {
	}

	void Renderer::beginFrame() {
        m_rendererData->getGBufferFramebuffer()->bind();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
	}

    void Renderer::endFrame(entt::registry& entityRegistry, Camera* camera) {
        renderLights(entityRegistry, camera);

		// Post processing
		m_rendererData->getLightingFramebuffer()->unbind();
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
        glDisable(GL_BLEND);

		m_rendererData->getRenderQuadVAO()->bind();
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

	void Renderer::setViewportSize(Vector2i viewportSize) {
        m_viewportSize = viewportSize;
        m_rendererData->setViewportSize(viewportSize);
		glViewport(0, 0, viewportSize.x, viewportSize.y);
	}

	void Renderer::setDefaultShader(ShaderAsset shaderAsset) {
		m_rendererData->setDefaultShader(shaderAsset);
	}

	void Renderer::renderModel(Model* model, TransformComponent* transform, Camera* camera) {
		for(auto& mesh : model->meshes) {
			if(mesh) {
				m_rendererData->getGeometryPassShader()->useShader();

				glActiveTexture(GL_TEXTURE0);
                if(mesh->material && mesh->material->texture) {
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

    void Renderer::renderLights(entt::registry& entityRegistry, Camera* camera) {
		m_rendererData->getLightingFramebuffer()->bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

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

        m_rendererData->getDefaultLightingShader()->setUniformMat4("u_viewMatrix", camera->getViewMatrix());
        m_rendererData->getDefaultLightingShader()->setUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());
        m_rendererData->getDefaultLightingShader()->setUniform3f("u_cameraPos", camera->position.x, camera->position.y, camera->position.z);
        m_rendererData->getDefaultLightingShader()->setUniform2f("u_viewportSize", m_viewportSize.x, m_viewportSize.y);

        auto lightComponentView = entityRegistry.view<LightComponent>();
        for(auto& entity : lightComponentView) {
            auto& lightComponent = lightComponentView.get<LightComponent>(entity);
			auto& transformComponent = entityRegistry.get<TransformComponent>(entity);

            m_rendererData->getDefaultLightingShader()->setUniform1ui("u_lightType", static_cast<unsigned int>(lightComponent.lightType));
            if(lightComponent.lightType == LightType::PointLight) {
                m_rendererData->getSphereVAO()->bind();
                glCullFace(GL_FRONT); // Render the inside of the light spheres

                Vector3 lightPos = transformComponent.getPosition();
                m_rendererData->getDefaultLightingShader()->setUniform3f("u_lightPos", lightPos.x, lightPos.y, lightPos.z);
                m_rendererData->getDefaultLightingShader()->setUniform1f("u_pointLightRange", lightComponent.pointLightRange);

                glDrawElements(GL_TRIANGLES, m_rendererData->getSphereIndexCount(), GL_UNSIGNED_INT, 0);
            }
            else if(lightComponent.lightType == LightType::DirectionalLight) {
                m_rendererData->getRenderQuadVAO()->bind();
                glCullFace(GL_BACK);

                Vector3 lightDir = - transformComponent.getForwardVector();
                m_rendererData->getDefaultLightingShader()->setUniform3f("u_lightDir", lightDir.x, lightDir.y, lightDir.z);
                m_rendererData->getDefaultLightingShader()->setUniform1f("u_lightIntensity", lightComponent.intensity);

                glDrawElements(GL_TRIANGLES, m_rendererData->getRenderQuadIndexCount(), GL_UNSIGNED_INT, 0);
            }


        }
    }


}
