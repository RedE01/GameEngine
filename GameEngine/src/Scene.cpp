#include "Scene.h"

namespace GameEngine {

	Scene::~Scene() {
		for(auto& go : m_gameObjects) {
			go.deinit();
		}
		m_gameObjects.clear();
	}

	GameObject* Scene::createGameObject() {
		GameObject* go = m_gameObjects.create();
		go->init();
		return go;
	}

	bool Scene::removeGameObject(Resource<GameObject> gameObjectResource) {
		std::optional<GameObject*> go = getGameObject(gameObjectResource);
		if(go.has_value()) go.value()->deinit();

		return m_gameObjects.remove(gameObjectResource.ID);
	}

	std::optional<GameObject*> Scene::getGameObject(Resource<GameObject> gameObjectResource) {
		return m_gameObjects.get(gameObjectResource.ID);
	}

	void Scene::updateGameObjects() {
		for(auto& go : m_gameObjects) {
			go.update();
		}
	}


}