#pragma once
#include "../Math/Vector.h"
#include <entt/entity/registry.hpp>

namespace GameEngine {

	class Camera;

	class Renderer {
	public:
		Renderer(Vector2 viewportSize);

		void beginFrame();
		void renderEntities(entt::registry& entityRegistry, Camera* camera);

		void setViewportSize(Vector2 viewportSize);
	};

}