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
			component.init();
			
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

        template <typename T>
        bool hasComponent() {
            assert(isValid());

            return m_entityRegistry->all_of<T>(getID());
        }

		bool isValid() const;
		entityid getID() const;

        void eachComponent(std::function<void(Component&)> function);

		bool operator==(const Entity& other) const;
		bool operator!=(const Entity& other) const;

	private:
		void initComponent(Component* component);

	private:
		entityid m_entityID = entt::null;
		entt::registry* m_entityRegistry;
	};

}
