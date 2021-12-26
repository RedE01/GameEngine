#include "ScriptComponentManager.h"

namespace GameEngine {

    std::vector<ScriptComponentManager::eachComponentFunctionType> ScriptComponentManager::m_eachComponentFunction;
    std::vector<ScriptComponentManager::eachComponentOfEntityFunctionType> ScriptComponentManager::m_eachComponentOfEntityFunction;

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

}
