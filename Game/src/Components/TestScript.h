#pragma once
#include <iostream>
#include "../../GameEngine/src/Components/Component.h"

using namespace GameEngine;

class TestScript : public Component {
public:
	void update() {
		std::cout << "Update TestScript" << std::endl;
	}
};