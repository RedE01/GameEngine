#include "Renderer.h"
#include "../Components/MeshRendererComponent.h"
#include "../Components/TransformComponent.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <GL/glew.h>

#include <gtc/matrix_transform.hpp>

namespace GameEngine {

	void renderModel(Model& model, TransformComponent& transform, Camera* camera);

	Renderer::Renderer(Vector2 viewportSize) {
		if(glewInit() != GLEW_OK) return;

		glEnable(GL_CULL_FACE);
		initializeRendererDebugger();

		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_FRAMEBUFFER_SRGB); // Temporary

		setViewportSize(viewportSize);
	}

	void Renderer::beginFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::renderEntities(entt::registry& entityRegistry, Camera* camera) {
		if(camera == nullptr) return;

		auto meshRendererView = entityRegistry.view<MeshRendererComponent>();
		for(auto& entity : meshRendererView) {
			auto& meshRendererComponent = meshRendererView.get<MeshRendererComponent>(entity);
			auto& transformComponent = entityRegistry.get<TransformComponent>(entity);

			if(meshRendererComponent.model) {
				renderModel(meshRendererComponent.model.get(), transformComponent, camera);
			}
		}
	}

	void Renderer::setViewportSize(Vector2 viewportSize) {
		glViewport(0, 0, viewportSize.x, viewportSize.y);
	}

	void renderModel(Model& model, TransformComponent& transform, Camera* camera) {
		for(auto& mesh : model.meshes) {
			if(mesh && mesh->material && mesh->material->shader) {
				mesh->material->shader->useShader();

				glActiveTexture(0);
				if(mesh->material->texture) {
					mesh->material->texture->bind();
				}

				Matrix4 modelMat = transform.getMatrix();

				mesh->material->shader->setUniformMat4("u_modelMatrix", modelMat);
				mesh->material->shader->setUniformMat4("u_viewMatrix", camera->getViewMatrix());
				mesh->material->shader->setUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());

				mesh->material->shader->setUniform3f("u_cameraPos", camera->position.x, camera->position.y, camera->position.z);

				mesh->bind();

				glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
			}
		}
	}


}