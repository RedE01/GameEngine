#pragma once
#include "Components/Component.h"
#include <entt/entity/registry.hpp>

namespace GameEngine {

	using entityid = entt::entity;

	class Entity {
	public:
		Entity(const Entity& other) = default;

		template<typename ComponentType, typename... Args>
		ComponentType& addComponent(Args&&... args) {
			ComponentType& component = m_entityRegistry->emplace<ComponentType>(m_entityID, std::forward<Args>(args)...);
			
			Component* componentPtr = (Component*)&component;
			componentPtr->m_entity = this;
			
			return component;
		}

		template <typename T>
		void removeComponent() {
			m_entityRegistry->remove<T>(m_entityID);
		}

		template <typename T>
		T& getComponent() {
			return m_entityRegistry->get<T>(m_entityID);
		}

		entityid getID() const;

	private:
		Entity(entt::registry* entityRegistry, entityid entityID);

	private:
		entityid m_entityID;
		entt::registry* m_entityRegistry;

		friend class Scene;
	};

}