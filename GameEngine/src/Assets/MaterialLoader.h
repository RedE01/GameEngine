#pragma once
#include "../Rendering/Material.h"
#include <entt/resource/loader.hpp>

namespace GameEngine {

	struct MaterialLoader final : entt::resource_loader<MaterialLoader, Material> {
		std::shared_ptr<Material> load() const;
	};

}


