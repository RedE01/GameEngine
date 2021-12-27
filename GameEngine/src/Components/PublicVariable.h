#pragma once
#include <variant>
#include <string>
#include <vector>
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace GameEngine {

    struct PublicEnum {
    public:
        PublicEnum(const int& selection, const std::vector<std::string>& options) : selection(selection), options(options) {}
        int selection = 0;
        std::vector<std::string> options;
    };

    struct PublicVariable {
    public:
        template <typename T>
        PublicVariable(std::string name, T& variable)
            : m_name(name), m_data(&variable) {
        }

        template <typename E>
        PublicVariable(std::string name, E& enumVariable, const std::vector<std::string>& options)
            : m_name(name), m_data(PublicEnum(static_cast<int>(enumVariable), options)) {
        }

        std::string getName() const { return m_name; }

        template <template <typename P> class Visitor>
        void visit() {
            std::visit([&](auto&& arg){
                using T = std::decay_t<decltype(arg)>;
                Visitor<T>::visit(getName(), arg);
            }, m_data);
        }

    private:
        std::string m_name;
        std::variant<int*, float*, double*, bool*, char*, std::string*, Vector2*, Vector3*, Vector4*, Vector2i*, Vector3i*, Vector4i*, Quaternion*, PublicEnum> m_data;
    };

}


