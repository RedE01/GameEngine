#include "Scene.h"

namespace GameEngine {

	Scene::~Scene() {
		m_gameObjects.clear();
	}

	GameObject* Scene::createGameObject() {
		GameObject* go = m_gameObjects.create();
		go->init();
		return go;
	}

	bool Scene::removeGameObject(Resource<GameObject> gameObjectResource) {
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