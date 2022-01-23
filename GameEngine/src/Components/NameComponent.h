#pragma once
#include "Component.h"
#include <string>

namespace GameEngine {

	class NameComponent : public Component {
	public:
        NameComponent(const char* name) : name(name) {}
        NameComponent(const std::string& name) : name(name) {}

        virtual std::string getName() const override { return name; }
        const char* getNameCStr() const { return name.c_str(); }

    public:
        std::string name;
	};

}

