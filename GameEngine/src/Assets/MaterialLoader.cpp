#include "MaterialLoader.h"

namespace GameEngine {

		std::shared_ptr<Material> MaterialLoader::load() const {
			return std::make_shared<Material>();
		}

}