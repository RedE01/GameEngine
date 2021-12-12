#pragma once
#include "../Rendering/Texture.h"
#include <entt/resource/loader.hpp>

namespace GameEngine {

	struct TextureLoader final : entt::resource_loader<TextureLoader, Texture> {
		std::shared_ptr<Texture> load(const std::string& filepath, bool srgb = true) const;
		std::shared_ptr<Texture> load(const std::string& identifier, unsigned char* textureData, unsigned int dataLength, bool srgb = true) const;
	};

}

