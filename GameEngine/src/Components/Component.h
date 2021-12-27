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
        PublicVariable& getPublicVariable(size_t index) {
            return m_publicVariables[index];
        }

    protected:
        Entity getEntity() const {
            return m_entity;
        }

        template <typename T>
        void registerPublicVariable(const std::string& name, T& variable) {
            m_publicVariables.emplace_back(PublicVariable(name, variable));
        }

        template <typename T>
        void registerPublicVariable(const std::string& name, T& variable, const std::vector<std::string>& options) {
            m_publicVariables.emplace_back(PublicVariable(name, variable, options));
        }

    private:
        Entity m_entity;
        std::vector<PublicVariable> m_publicVariables;

        friend class Entity;
    };

}
