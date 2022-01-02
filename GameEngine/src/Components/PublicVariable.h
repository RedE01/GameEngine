#pragma once
#include <variant>
#include <vector>
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Assets/AssetHandle.h"

namespace GameEngine {

    class Component;

    struct PublicVariable {
    public:
        template <typename VariableType, class ComponentType>
        PublicVariable(VariableType ComponentType::* variable)
            : m_data(static_cast<VariableType Component::*>(variable)) {
        }

        template <typename EnumType, class ComponentType>
            requires std::is_enum<EnumType>::value
        PublicVariable(EnumType ComponentType::* enumVariable, const std::vector<std::string>& options)
            : m_data(PublicEnum(reinterpret_cast<int Component::*>(enumVariable), options)) {
        }

        template <template <typename P> class Visitor, typename... Args>
        void visit(Component& component, Args&&... args) {
            std::visit([&](auto&& arg){
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same<T, PublicEnum>()) {
                    Visitor<T>::visit(&(component.*(arg.selection)), arg.options, std::forward<Args>(args)...);
                }
                else {
                    Visitor<T>::visit(&(component.*arg), std::forward<Args>(args)...);
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

        template <typename... Ts> using variantType = std::variant<Ts Component::*..., PublicEnum>;
        variantType<int, float, double, bool, char, std::string, Vector2, Vector3, Vector4, Vector2i, Vector3i, Vector4i, Quaternion, ModelAsset, ShaderAsset, TextureAsset, MaterialAsset> m_data;
    };

}


