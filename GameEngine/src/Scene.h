#pragma once
#include "Entity.h"

namespace GameEngine {
	
	class ScriptComponentManager;

	class Scene {
	public:
		Entity createEntity();
		void destroyEntity(const Entity& entity);

	private:
		void update(ScriptComponentManager* scriptComponentManager);
		void updateCameras(float windowWidth, float windowHeight);

	private:
		entt::registry m_entityRegistry;

		friend class Application;
		friend class Renderer;
	};

}