#pragma once
#include <variant>
#include <string>
#include <vector>
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace GameEngine {

    class Component;

    struct PublicVariable {
    public:
        template <typename VariableType, class ComponentType>
        PublicVariable(std::string name, VariableType ComponentType::* variable)
            : m_name(name), m_data(static_cast<VariableType Component::*>(variable)) {
        }

        template <typename EnumType, class ComponentType>
            requires std::is_enum<EnumType>::value
        PublicVariable(std::string name, EnumType ComponentType::* enumVariable, const std::vector<std::string>& options)
            : m_name(name), m_data(PublicEnum(reinterpret_cast<int Component::*>(enumVariable), options)) {
        }

        std::string getName() const { return m_name; }

        template <template <typename P> class Visitor>
        void visit(Component& component) {
            std::visit([&](auto&& arg){
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same<T, PublicEnum>()) {
                    Visitor<T>::visit(getName(), &(component.*(arg.selection)), arg.options);
                }
                else {
                    Visitor<T>::visit(getName(), &(component.*arg));
                }
            }, m_data);
        }

    private:

        struct PublicEnum {
        public:
            PublicEnum(int Component::* selection, const std::vector<std::string>& options) : selection(selection), options(options) {}
            int Component::* selection;
            std::vector<std::string> options;
        };

        std::string m_name;
        template <typename... Ts> using variantType = std::variant<Ts Component::*..., PublicEnum>;
        variantType<int, float, double, bool, char, std::string, Vector2, Vector3, Vector4, Vector2i, Vector3i, Vector4i, Quaternion> m_data;
    };

}


