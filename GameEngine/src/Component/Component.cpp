#include "Component.h"
#include <iostream>

namespace GameEngine {

	Component::Component(unsigned int id) : m_id(id) {
	}

	void Component::init() {
		std::cout << "Component " << m_id << " init" << std::endl;
	}

	void Component::update() {
		std::cout << "Component " << m_id << " update" << std::endl;
	}

	const unsigned int Component::getID() const {
		return m_id;
	}

}