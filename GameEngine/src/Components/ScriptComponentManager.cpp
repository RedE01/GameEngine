#include "ScriptComponentManager.h"

namespace GameEngine {

    std::vector<ScriptComponentManager::eachComponentFunctionType> ScriptComponentManager::m_eachComponentFunction;
    std::vector<ScriptComponentManager::eachComponentOfEntityFunctionType> ScriptComponentManager::m_eachComponentOfEntityFunction;
    std::unordered_map<std::string, ScriptComponentManager::createComponentFunctionType> ScriptComponentManager::m_createComponentFunctions;

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

    Component* ScriptComponentManager::createComponent(const std::string& componentTypeName, Entity& entity) {
        auto search = m_createComponentFunctions.find(componentTypeName);
        if(search != m_createComponentFunctions.end()) {
            return search->second(entity);
        }
        return nullptr;
    }

}
