#pragma once
#include "GameObject.h"
#include "Util/ResourceArray.h"
#include "Util/Resource.h"

namespace GameEngine {

	class Scene {
	public:
		~Scene();

		GameObject* createGameObject();
		bool removeGameObject(Resource<GameObject> gameObjectResource);
		std::optional<GameObject*> getGameObject(Resource<GameObject> gameObjectResource);

		void updateGameObjects();

	private:
		ResourceArray<GameObject> m_gameObjects;
	};

}