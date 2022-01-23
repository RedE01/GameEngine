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

    template<class T>
    bool tryDeleteComponentFromName(Entity& entity, const std::string& componentTypeName);

	Entity::Entity() : m_entityID(entt::null), m_entityRegistry(nullptr) {
	}

	Entity::Entity(entt::registry* entityRegistry, entityid entityID) 
		: m_entityID(entityID), m_entityRegistry(entityRegistry) {

	}

    void Entity::removeComponent(Component& component) {
        assert(isValid());

        std::string componentTypeName = component.getName();
        std::cout << "removing: " << componentTypeName << std::endl;
        if(tryDeleteComponentFromName<CameraComponent>(*this, componentTypeName)) return;
        if(tryDeleteComponentFromName<LightComponent>(*this, componentTypeName)) return;
        if(tryDeleteComponentFromName<MeshRendererComponent>(*this, componentTypeName)) return;

        ScriptComponentManager::removeComponent(componentTypeName, *this);
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

    template<class T>
    bool tryDeleteComponentFromName(Entity& entity, const std::string& componentTypeName) {
        if(entity.hasComponent<T>() && entity.getComponent<T>().getName() == componentTypeName) {
            entity.removeComponent<T>();
            return true;
        }
        return false;
    }

}
