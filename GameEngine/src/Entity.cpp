#include "Entity.h"
#include "Components/Component.h"

namespace GameEngine {

	Entity::Entity() : m_entityID(entt::null), m_entityRegistry(nullptr) {
	}

	Entity::Entity(entt::registry* entityRegistry, entityid entityID) 
		: m_entityID(entityID), m_entityRegistry(entityRegistry) {

	}

	bool Entity::isValid() const {
		if(m_entityRegistry == nullptr) return false;
		return m_entityRegistry->valid(getID());
	}

	entityid Entity::getID() const {
		return m_entityID;
	}

	bool Entity::operator==(const Entity& other) const {
		return getID() == other.getID();
	}

	bool Entity::operator!=(const Entity& other) const {
		return getID() != other.getID();
	}

	void Entity::initComponent(Component* component) {
		component->m_entity = *this;
	}


}
