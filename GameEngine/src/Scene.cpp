#include "Scene.h"
#include "Components/ScriptComponentManager.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

namespace GameEngine {

	Entity Scene::createEntity() {
		entt::entity entity = m_entityRegistry.create();
		
		Entity e(&m_entityRegistry, entity);
		e.addComponent<TransformComponent>();

		return e;
	}

	void Scene::destroyEntity(const Entity& entity) {
		m_entityRegistry.destroy(entity.getID());
	}

	void Scene::update(ScriptComponentManager* scriptComponentManager) {
		scriptComponentManager->updateScriptComponents(m_entityRegistry);
	}

	void Scene::updateCameras(float windowWidth, float windowHeight) {
		auto cameraView = m_entityRegistry.view<CameraComponent>();

		for(auto& entity : cameraView) {
			CameraComponent& cameraComponent = cameraView.get<CameraComponent>(entity);

			cameraComponent.setSize(Vector2(windowWidth, windowHeight));
		}
	}
}