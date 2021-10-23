#pragma once
#include <entt/entity/registry.hpp>

namespace GameEngine {

	using entityid = entt::entity;

	class Entity {
	public:
		Entity(const Entity& other) = default;

		template<typename Component, typename... Args>
		Component& addComponent(Args&&... args) {
			return m_entityRegistry->emplace<Component>(m_entityID, std::forward<Args>(args)...);
		}

		template <class T>
		void removeComponent() {
			m_entityRegistry->remove<T>(m_entityID);
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