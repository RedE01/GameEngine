#pragma once
#include "../Rendering/Shader.h"
#include <memory>

namespace GameEngine {

	struct ShaderLoader {
		std::shared_ptr<Shader> load(const std::string& filepath) const;
	};

}
