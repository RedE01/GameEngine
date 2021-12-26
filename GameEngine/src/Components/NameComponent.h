#pragma once
#include "Component.h"
#include <string>

namespace GameEngine {

	class NameComponent : public Component {
	public:
        NameComponent(const char* name) : m_name(name) {}
        NameComponent(const std::string& name) : m_name(name) {}

        virtual std::string getName() const override { return m_name; }
        const char* getNameCStr() const { return m_name.c_str(); }

    private:
        std::string m_name;
	};

}

