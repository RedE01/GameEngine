#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

namespace GameEngine {

	std::shared_ptr<Texture> loadTexture(unsigned char* data, int imageWidth, int imageHeight, int channels, const std::string& identifier, bool srgb) {
		TextureFormat textureFormat;
		switch(channels) {
		case 1: textureFormat = TextureFormat::R; break;
		case 2: textureFormat = TextureFormat::RG; break;
        case 3: textureFormat = srgb ? TextureFormat::SRGB : TextureFormat::RGB; break;
        case 4: textureFormat = srgb ? TextureFormat::SRGBA : TextureFormat::RGBA; break;
		default:
			std::cout << "Number of texture channels unsuported for texture: " << identifier << std::endl;
			stbi_image_free(data);
			return std::shared_ptr<Texture>();
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->textureImage2D(textureFormat, imageWidth, imageHeight, data);

		texture->setFilterMode(TextureFilterMode::LINEAR);
		texture->setWrapModeS(TextureWrapMode::REPEAT);
		texture->setWrapModeT(TextureWrapMode::REPEAT);

		stbi_image_free(data);

		return texture;		
	}

	std::shared_ptr<Texture> TextureLoader::load(AssetData<Texture>* assetData) const {
        const std::string& filepath = assetData->filepath;
        const bool& srgb = assetData->srgb;

		stbi_set_flip_vertically_on_load(true);

		int imageWidth;
		int imageHeight;
		int channels;
		unsigned char* data = stbi_load(filepath.c_str(), &imageWidth, &imageHeight, &channels, 0);
		if(!data) {
			std::cout << "Could not load texture: " << filepath << std::endl;
			return std::shared_ptr<Texture>();
		}

		return loadTexture(data, imageWidth, imageHeight, channels, filepath, srgb);

	}

	std::shared_ptr<Texture> TextureLoader::load(AssetData<Texture>* assetData, unsigned char* textureData, unsigned int dataLength) const {
        const std::string& name = assetData->name;
        const bool& srgb = assetData->srgb;

		stbi_set_flip_vertically_on_load(true);

		int imageWidth;
		int imageHeight;
		int channels;
		unsigned char* data = stbi_load_from_memory(textureData, dataLength, &imageWidth, &imageHeight, &channels, 0);
		if(!data) {
			std::cout << "Could not load texture from memory: " << name << std::endl;
			return std::shared_ptr<Texture>();
		}

		return loadTexture(data, imageWidth, imageHeight, channels, name, srgb);
	}

}
