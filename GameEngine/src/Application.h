#pragma once
#include <string>

namespace GameEngine {

	class Application {
	public:
		Application(const std::string& name);

		void run();
	};

}