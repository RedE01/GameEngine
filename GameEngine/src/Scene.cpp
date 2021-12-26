#include "Scene.h"
#include "Components/ScriptComponentManager.h"
#include "Components/TransformComponent.h"
#include "Components/NameComponent.h"
#include "Components/CameraComponent.h"

namespace GameEngine {

	CameraComponent* getActiveCameraComponent(entt::registry& registry);

	Entity Scene::createEntity(const std::string& name) {
		entt::entity entity = m_entityRegistry.create();
		
		Entity e(&m_entityRegistry, entity);
		e.addComponent<NameComponent>(name);
		e.addComponent<TransformComponent>();

		return e;
	}

	void Scene::destroyEntity(const Entity& entity) {
		m_entityRegistry.destroy(entity.getID());
	}

	Camera* Scene::getActiveCamera() {
		CameraComponent* cameraComponent = getActiveCameraComponent(m_entityRegistry);

		if(cameraComponent != nullptr) return &(cameraComponent->getCamera());
		return nullptr;
	}

	void Scene::update() {
        ScriptComponentManager::eachComponent(m_entityRegistry, [](Component& component){
            component.update();
        });

		CameraComponent* cameraComponent = getActiveCameraComponent(m_entityRegistry);
		if(cameraComponent != nullptr) cameraComponent->updateCamera();
	}

	void Scene::updateCameras(float windowWidth, float windowHeight) {
		auto cameraView = m_entityRegistry.view<CameraComponent>();

		for(auto& entity : cameraView) {
			CameraComponent& cameraComponent = cameraView.get<CameraComponent>(entity);

			cameraComponent.getCamera().setSize(Vector2(windowWidth, windowHeight));
		}
	}

	CameraComponent* getActiveCameraComponent(entt::registry& registry) {
		auto cameraView = registry.view<CameraComponent>();

		for(auto& entity : cameraView) {
			CameraComponent& cameraComponent = cameraView.get<CameraComponent>(entity);

			if(cameraComponent.getCamera().isActive) {
				return &cameraComponent;
			}
		}

		return nullptr;
	}
}
