#include "Scene.h"

namespace GameEngine {

	Entity Scene::createEntity() {
		entt::entity entity = m_entityRegistry.create();
		return Entity(&m_entityRegistry, entity);
	}

	void Scene::destroyEntity(const Entity& entity) {
		m_entityRegistry.destroy(entity.getID());
	}
}