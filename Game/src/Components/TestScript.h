#pragma once
#include <iostream>

class TestScript {
public:
	void update() {
		std::cout << "Update TestScript" << std::endl;
	}

	int test = 3;
};