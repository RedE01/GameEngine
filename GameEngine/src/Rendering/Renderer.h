#pragma once
#include "../Math/Vector.h"
#include <entt/entity/registry.hpp>
#include <memory>

namespace GameEngine {

	class Camera;
	class RendererData;

	class Renderer {
	public:
		Renderer(Vector2 viewportSize);
		~Renderer();

		void beginFrame();
		void endFrame();
		void renderEntities(entt::registry& entityRegistry, Camera* camera);

		void setViewportSize(Vector2 viewportSize);

	private:
		std::unique_ptr<RendererData> m_rendererData;
	};

}