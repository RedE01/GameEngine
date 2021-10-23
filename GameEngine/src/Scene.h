#pragma once
#include "Entity.h"

namespace GameEngine {
	
	class Scene {
	public:
		Entity createEntity();
		void destroyEntity(const Entity& entity);

	private:
		entt::registry m_entityRegistry;

		friend class Entity;
		friend class Application;
	};

}