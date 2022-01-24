#include "Renderer.h"
#include "../Components/MeshRendererComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/TransformComponent.h"
#include "../Scene/Scene.h"
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
        if(m_viewportSize.x == 0 || m_viewportSize.y == 0) return;

        m_rendererData->getGBufferFramebuffer()->bind();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
	}

    void Renderer::endFrame(Scene* scene, Camera* camera) {
        if(m_viewportSize.x == 0 || m_viewportSize.y == 0 || camera == nullptr) return;

        renderLights(scene, camera);

		// Post processing
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
        glDisable(GL_BLEND);

        m_rendererData->getPostProcessingFramebuffer()->bind();
        m_rendererData->getRenderQuadVAO()->bind();
        m_rendererData->getPostProcessingShader()->useShader();
        glActiveTexture(GL_TEXTURE0);
        m_rendererData->getLightingTexture()->bind();
        m_rendererData->getPostProcessingShader()->setUniform1f("u_exposure", camera->exposure);
        m_rendererData->getPostProcessingShader()->setUniform1i("u_frameTexture", 0);

        glDrawElements(GL_TRIANGLES, m_rendererData->getRenderQuadIndexCount(), GL_UNSIGNED_INT, 0);

        m_rendererData->getPostProcessingFramebuffer()->unbind();
	}

    void Renderer::renderFrameToDefaultFramebuffer() {
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
        glDisable(GL_BLEND);

		m_rendererData->getPostProcessingFramebuffer()->unbind();
		m_rendererData->getRenderQuadVAO()->bind();
		m_rendererData->getFinalRenderShader()->useShader();
		glActiveTexture(GL_TEXTURE0);
        m_rendererData->getFrameTexture()->bind();
		m_rendererData->getPostProcessingShader()->setUniform1i("u_frameTexture", 0);

		glDrawElements(GL_TRIANGLES, m_rendererData->getRenderQuadIndexCount(), GL_UNSIGNED_INT, 0);

    }

	void Renderer::renderEntities(Scene* scene, Camera* camera) {
		if(m_viewportSize.x == 0 || m_viewportSize.y == 0 || camera == nullptr) return;

        scene->eachInGroup<TransformComponent, MeshRendererComponent>([&](TransformComponent& transformComponent, MeshRendererComponent& meshRendererComponent){
			if(meshRendererComponent.model) {
				renderModel(&meshRendererComponent.model.get(), &transformComponent, camera);
			}
        });
	}

	void Renderer::setViewportSize(Vector2i viewportSize) {
        if(viewportSize.x < 0) viewportSize.x = 0;
        if(viewportSize.y < 0) viewportSize.y = 0;

        m_viewportSize = viewportSize;
        m_rendererData->setViewportSize(viewportSize);
        glViewport(0, 0, viewportSize.x, viewportSize.y);
	}

    std::shared_ptr<Texture> Renderer::getFrameTexture() {
        return m_rendererData->getFrameTexture();
    }

	void Renderer::renderModel(Model* model, TransformComponent* transform, Camera* camera) {
		for(auto& mesh : model->meshes) {
			if(mesh) {
				m_rendererData->getGeometryPassShader()->useShader();
				m_rendererData->getGeometryPassShader()->setUniform1i("u_diffuseTexture", 0);
				m_rendererData->getGeometryPassShader()->setUniform1i("u_normalTexture", 1);

                if(mesh->material && mesh->material->diffuseTexture) {
                    glActiveTexture(GL_TEXTURE0);
                    mesh->material->diffuseTexture->bind();
                }

                if(mesh->material && mesh->material->normalTexture) {
                    glActiveTexture(GL_TEXTURE1);
                    mesh->material->normalTexture->bind();
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

    void Renderer::renderLights(Scene* scene, Camera* camera) {
		m_rendererData->getLightingFramebuffer()->bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        if(m_rendererData->getDefaultLightingShader()) {
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

            scene->each<LightComponent>([&](Entity entity, LightComponent& lightComponent){
                auto& transformComponent = entity.getComponent<TransformComponent>();

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

            });
        }

    }


}
