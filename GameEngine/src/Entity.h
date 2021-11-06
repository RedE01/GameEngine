#pragma once
#include <entt/entity/registry.hpp>
#include <type_traits>
#include <cassert>

namespace GameEngine {

	class Component;

	using entityid = entt::entity;

	class Entity {
	public:
		Entity();
		Entity(entt::registry* entityRegistry, entityid entityID);

		template<typename ComponentType, typename... Args>
		ComponentType& addComponent(Args&&... args) {
			static_assert(std::is_base_of<Component, ComponentType>::value, "Component type is not a subclass of Component");
			assert(isValid());

			ComponentType& component = m_entityRegistry->emplace<ComponentType>(m_entityID, std::forward<Args>(args)...);

			initComponent((Component*)&component);
			
			return component;
		}

		template <typename T>
		void removeComponent() {
			assert(isValid());

			m_entityRegistry->remove<T>(m_entityID);
		}

		template <typename T>
		T& getComponent() {
			assert(isValid());

			return m_entityRegistry->get<T>(m_entityID);
		}

		bool isValid() const;
		entityid getID() const;

		bool operator==(const Entity& other);
		bool operator!=(const Entity& other);

	private:
		void initComponent(Component* component);

	private:
		entityid m_entityID = entt::null;
		entt::registry* m_entityRegistry;

		friend class Scene;
	};

}