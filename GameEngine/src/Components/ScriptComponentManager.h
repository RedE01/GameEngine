#pragma once
#include "Component.h"
#include <vector>
#include <functional>
#include <entt/entity/registry.hpp>

namespace GameEngine {

	namespace ScriptComponentManagerInternal {
		template <class T>
		void updateComponentsOfType(entt::registry& registry) {
			registry.view<T>().each([](T& component){
				component.update();
			});
		}
	}

	class ScriptComponentManager {
	public:
		template<typename T>
		void registerScriptComponent() {
			m_scriptComponentUpdateFunctions.push_back(ScriptComponentManagerInternal::updateComponentsOfType<T>);
		}

	private:
		void updateScriptComponents(entt::registry& registry) {
			for(auto& updateFn : m_scriptComponentUpdateFunctions) {
				updateFn(registry);
			}
		}

	private:
		std::vector<std::function<void(entt::registry&)>> m_scriptComponentUpdateFunctions;

		friend class Scene;
	};

}