#pragma once
#include "../Rendering/Texture.h"
#include <memory>

namespace GameEngine {

	struct TextureLoader {
		std::shared_ptr<Texture> load(const std::string& filepath, bool srgb = true) const;
		std::shared_ptr<Texture> load(const std::string& identifier, unsigned char* textureData, unsigned int dataLength, bool srgb = true) const;
	};

}

