#include "GameObject.h"
#include <iostream>

namespace GameEngine {

	GameObject::GameObject(unsigned int id) : m_id(id) {
	}

	void GameObject::init() {
		std::cout << "Game Object " << m_id << " init" << std::endl;
	}

	void GameObject::deinit() {
		std::cout << "Game Object " << m_id << " deinit" << std::endl;
	}

	void GameObject::update() {
		std::cout << "Game Object " << m_id << " update" << std::endl;
	}

	const unsigned int GameObject::getID() const {
		return m_id;
	}


}