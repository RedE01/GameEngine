#include "Entity.h"

namespace GameEngine {

	Entity::Entity(entt::registry* entityRegistry, entityid entityID) 
		: m_entityID(entityID), m_entityRegistry(entityRegistry) {

	}

	entityid Entity::getID() const {
		return m_entityID;
	}

}