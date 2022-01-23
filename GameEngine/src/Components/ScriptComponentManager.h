#pragma once
#include "Component.h"
#include "../Entity.h"
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

            T instance;
            m_componentTypeNames.push_back(instance.getName());

            m_createComponentFunctions.insert({
                instance.getName(),
                [](Entity& entity) -> Component* {
                    return static_cast<Component*>(&(entity.addComponent<T>()));
                }
            });
		}

		static void eachComponent(entt::registry& registry, std::function<void(Component&)> function);
		static void eachComponent(Entity entity, std::function<void(Component&)> function);
        static void eachComponentTypeName(std::function<void(const std::string&)> function);
        static Component* createComponent(const std::string& componentTypeName, Entity& entity);

	private:
        using eachComponentFunctionType = std::function<void(entt::registry&, std::function<void(Component&)>)>;
        using eachComponentOfEntityFunctionType = std::function<void(Entity, std::function<void(Component&)>)>;
        //using eachComponentTypeFunctionType = std::function<void(const std::)>
        using createComponentFunctionType = std::function<Component*(Entity&)>;
		static std::vector<eachComponentFunctionType> m_eachComponentFunction;
		static std::vector<eachComponentOfEntityFunctionType> m_eachComponentOfEntityFunction;
        static std::vector<std::string> m_componentTypeNames;
        static std::unordered_map<std::string, createComponentFunctionType> m_createComponentFunctions;
	};

}
