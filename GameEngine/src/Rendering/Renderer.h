#pragma once
#include "../Math/Vector.h"
#include <entt/entity/registry.hpp>
#include <memory>

namespace GameEngine {

	class Camera;
	class RendererData;
	class Model;
	class TransformComponent;

	class Renderer {
	public:
		Renderer(Vector2 viewportSize);
		~Renderer();

		void beginFrame();
		void endFrame();
		void renderEntities(entt::registry& entityRegistry, Camera* camera);

		void setViewportSize(Vector2 viewportSize);

	private:
		void renderModel(Model* model, TransformComponent* transform, Camera* camera);

	private:
		std::unique_ptr<RendererData> m_rendererData;
	};

}