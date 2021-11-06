#include "Renderer.h"
#include "../Scene.h"
#include "../Components/CameraComponent.h"
#include "../Components/MeshRendererComponent.h"
#include "../Components/TransformComponent.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>

#include <gtc/matrix_transform.hpp>

namespace GameEngine {

	struct CameraMatrices {
		Matrix4 viewMatrix;
		Matrix4 projectionMatrix;
	};

	CameraMatrices getCameraMatrices(entt::registry& registry);
	void renderModel(Model& model, TransformComponent& transform, CameraMatrices& cameraMatrices);

	Renderer::Renderer() {
		if(glewInit() != GLEW_OK) return;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_FRAMEBUFFER_SRGB); // Temporary
	}

	void Renderer::renderFrame(Scene* scene) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(scene != nullptr) {
			CameraMatrices cameraMatrices = getCameraMatrices(scene->m_entityRegistry);

			auto meshRendererView = scene->m_entityRegistry.view<MeshRendererComponent>();
			for(auto& entity : meshRendererView) {
				auto& meshRendererComponent = meshRendererView.get<MeshRendererComponent>(entity);
				auto& transformComponent = scene->m_entityRegistry.get<TransformComponent>(entity);

				if(meshRendererComponent.model) {
					renderModel(meshRendererComponent.model.get(), transformComponent, cameraMatrices);
				}
			}
		}
	}

	CameraMatrices getCameraMatrices(entt::registry& registry) {
		auto cameraView = registry.view<CameraComponent>();
		for(auto& entity : cameraView) {
			CameraComponent& cameraComponent = cameraView.get<CameraComponent>(entity);

			if(cameraComponent.isActive) {
				return {cameraComponent.getViewMatrix(), cameraComponent.getProjectionMatrix()};
			}
		}

		return {Matrix4(0.0), Matrix4(0.0)};
	}

	void renderModel(Model& model, TransformComponent& transform, CameraMatrices& cameraMatrices) {
		for(auto& mesh : model.meshes) {
			if(mesh && mesh->material && mesh->material->shader) {
				mesh->material->shader->useShader();

				glActiveTexture(0);
				if(mesh->material->texture) {
					mesh->material->texture->bind();
				}

				Matrix4 modelMat = transform.getMatrix();

				mesh->material->shader->setUniformMat4("u_modelMatrix", modelMat);
				mesh->material->shader->setUniformMat4("u_viewMatrix", cameraMatrices.viewMatrix);
				mesh->material->shader->setUniformMat4("u_projectionMatrix", cameraMatrices.projectionMatrix);

				mesh->bind();

				glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
			}
		}
	}


}