#include "Renderer.h"
#include "../Scene.h"
#include "../Components/MeshRendererComponent.h"
#include "../Components/TransformComponent.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>

#include <gtc/matrix_transform.hpp>

namespace GameEngine {

	void renderModel(Model& model, TransformComponent& transform);

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
			auto view = scene->m_entityRegistry.view<MeshRendererComponent>();
			for(auto& entity : view) {
				auto& meshRendererComponent = view.get<MeshRendererComponent>(entity);
				auto& transformComponent = scene->m_entityRegistry.get<TransformComponent>(entity);

				if(meshRendererComponent.model) {
					renderModel(meshRendererComponent.model.get(), transformComponent);
				}
			}
		}
	}

	void renderModel(Model& model, TransformComponent& transform) {
		for(auto& mesh : model.meshes) {
			if(mesh && mesh->material && mesh->material->shader) {
				mesh->material->shader->useShader();

				glActiveTexture(0);
				if(mesh->material->texture) {
					mesh->material->texture->bind();
				}

				Matrix4 viewMat = glm::translate(glm::mat4(1.0), Vector3(0.0, 0.0, -10.0));
				Matrix4 projMat = glm::perspective(45.0f, 64.0f / 48.0f, 0.1f, 100.0f);
				Matrix4 modelMat = transform.getMatrix();

				mesh->material->shader->setUniformMat4("u_viewMat", viewMat);
				mesh->material->shader->setUniformMat4("u_projMat", projMat);
				mesh->material->shader->setUniformMat4("u_modelMat", modelMat);

				mesh->bind();

				glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
			}
		}
	}


}