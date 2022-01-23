#include "ScriptComponentManager.h"

namespace GameEngine {

    std::vector<ScriptComponentManager::eachComponentFunctionType> ScriptComponentManager::m_eachComponentFunction;
    std::vector<ScriptComponentManager::eachComponentOfEntityFunctionType> ScriptComponentManager::m_eachComponentOfEntityFunction;
    std::vector<std::string> ScriptComponentManager::m_componentTypeNames;
    std::unordered_map<std::string, ScriptComponentManager::createComponentFunctionType> ScriptComponentManager::m_createComponentFunctions;
    std::unordered_map<std::string, ScriptComponentManager::removeComponentFunctionType> ScriptComponentManager::m_removeComponentFunctions;

    void ScriptComponentManager::eachComponent(entt::registry& registry, std::function<void(Component&)> function) {
        for(auto& componentFunction : m_eachComponentFunction) {
            componentFunction(registry, function);
        }
    }

    void ScriptComponentManager::eachComponent(Entity entity, std::function<void(Component&)> function) {
        for(auto& componentFunction : m_eachComponentOfEntityFunction) {
            componentFunction(entity, function);
        }
    }

    void ScriptComponentManager::eachComponentTypeName(std::function<void(const std::string&)> function) {
        for(auto& componentTypeName : m_componentTypeNames) {
            function(componentTypeName);
        }
    }

    Component* ScriptComponentManager::createComponent(const std::string& componentTypeName, Entity& entity) {
        auto search = m_createComponentFunctions.find(componentTypeName);
        if(search != m_createComponentFunctions.end()) {
            return search->second(entity);
        }
        return nullptr;
    }

    bool ScriptComponentManager::removeComponent(const std::string& componentTypeName, Entity& entity) {
        auto search = m_removeComponentFunctions.find(componentTypeName);
        if(search != m_removeComponentFunctions.end()) {
            search->second(entity);
            return true;
        }
        return false;
    }
}
