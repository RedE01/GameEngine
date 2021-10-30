#pragma once
#include "../Rendering/Shader.h"
#include <entt/resource/loader.hpp>

namespace GameEngine {

	struct ShaderLoader final : entt::resource_loader<ShaderLoader, Shader> {
		std::shared_ptr<Shader> load(const std::string& filepath) const;
	};

}
