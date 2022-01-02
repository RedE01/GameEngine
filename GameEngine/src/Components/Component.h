#pragma once
#include "../Entity.h"
#include "PublicVariable.h"

namespace GameEngine {

    class Component {
    public:
        virtual ~Component() = default;
        virtual std::string getName() const = 0;

        virtual void init() {}
        virtual void update() {}

        size_t getPublicVariableCount() const { return m_publicVariables.size(); }

        PublicVariable* getPublicVariable(const std::string& name) {
            auto search = m_publicVariables.find(name);
            if(search != m_publicVariables.end()) return &(search->second);
            return nullptr;
        }

        void eachPublicVariable(std::function<void(const std::string&, PublicVariable&)> function) {
            for(auto& pv : m_publicVariables) function(pv.first, pv.second);
        }

    protected:
        Entity getEntity() const {
            return m_entity;
        }

        template <typename VariableType, class ComponentType>
        void registerPublicVariable(const std::string& name, VariableType ComponentType::* variable) {
            m_publicVariables.insert({name, PublicVariable(variable)});
        }

        template <typename EnumType, class ComponentType>
            requires std::is_enum<EnumType>::value
        void registerPublicVariable(const std::string& name, EnumType ComponentType::* variable, const std::vector<std::string>& options) {
            m_publicVariables.insert({name, PublicVariable(variable, options)});
        }

    private:
        Entity m_entity;
        std::unordered_map<std::string, PublicVariable> m_publicVariables;

        friend class Entity;
    };

}
