#pragma once
#include "../Rendering/Material.h"

namespace GameEngine {

	struct MaterialLoader {
		std::shared_ptr<Material> load() const;
	};

}


