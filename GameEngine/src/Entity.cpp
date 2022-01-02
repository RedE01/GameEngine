#include "Entity.h"
#include "Components/Component.h"
#include "Components/ScriptComponentManager.h"

#include "Components/NameComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshRendererComponent.h"

namespace GameEngine {

    template<class T>
    void handleComponent(Entity& entity, std::function<void(Component&)> function);

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

    void Entity::eachComponent(std::function<void(Component&)> function) {
        handleComponent<TransformComponent>(*this, function);
        handleComponent<CameraComponent>(*this, function);
        handleComponent<LightComponent>(*this, function);
        handleComponent<MeshRendererComponent>(*this, function);

        ScriptComponentManager::eachComponent(*this, function);
    }

    template<class T>
    void handleComponent(Entity& entity, std::function<void(Component&)> function) {
        if(entity.hasComponent<T>()) {
            function(entity.getComponent<T>());
        }
    };


}
