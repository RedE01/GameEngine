#pragma once
#include <entt/entity/registry.hpp>

namespace GameEngine {

	class Camera;

	class Renderer {
	public:
		Renderer();

		void beginFrame();
		void renderEntities(entt::registry& entityRegistry, Camera* camera);
	};

}