#include "Entity.h"

namespace GameEngine {

	Entity::Entity(entt::registry* entityRegistry, entityid entityID) 
		: m_entityRegistry(entityRegistry), m_entityID(entityID) {

	}

	entityid Entity::getID() const {
		return m_entityID;
	}

}