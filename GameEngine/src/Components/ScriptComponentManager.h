#pragma once
#include "Component.h"
#include <vector>
#include <functional>
#include <entt/entity/registry.hpp>

namespace GameEngine {

	class ScriptComponentManager {
	public:
        ScriptComponentManager() = delete;

		template<typename T>
		static void registerScriptComponent() {
            m_eachComponentFunction.push_back([](entt::registry& registry, std::function<void(Component&)> function){
                registry.view<T>().each([&](T& component){
                    function(component);
                });
            });

            m_eachComponentOfEntityFunction.push_back([](Entity entity, std::function<void(Component&)> function){
                if(entity.hasComponent<T>()) {
                    function(entity.getComponent<T>());
                }
            });
		}

		static void eachComponent(entt::registry& registry, std::function<void(Component&)> function);
		static void eachComponent(Entity entity, std::function<void(Component&)> function);

	private:
        using eachComponentFunctionType = std::function<void(entt::registry&, std::function<void(Component&)>)>;
        using eachComponentOfEntityFunctionType = std::function<void(Entity, std::function<void(Component&)>)>;
		static std::vector<eachComponentFunctionType> m_eachComponentFunction;
		static std::vector<eachComponentOfEntityFunctionType> m_eachComponentOfEntityFunction;
	};

}
