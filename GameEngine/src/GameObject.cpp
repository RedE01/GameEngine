#include "GameObject.h"
#include <iostream>

namespace GameEngine {

	GameObject::GameObject(unsigned int id) : m_id(id) {
	}

	void GameObject::init() {
		std::cout << "Game Object " << m_id << " init" << std::endl;
	}

	void GameObject::update() {
		std::cout << "Game Object " << m_id << " update" << std::endl;
	}

	Component* GameObject::createComponent() {
		Component* component = m_components.create();
		component->init();
		return component;
	}

	bool GameObject::removeComponent(Resource<Component> componentResource) {
		return m_components.remove(componentResource.ID);
	}

	std::optional<Component*> GameObject::getComponent(Resource<Component> componentResource) {
		return m_components.get(componentResource.ID);
	}

	const unsigned int GameObject::getID() const {
		return m_id;
	}


}