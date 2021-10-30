#pragma once
#include "ModelLoader.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include <entt/resource/cache.hpp>

namespace GameEngine {

	template<typename T>
	using AssetHandle = entt::resource_handle<T>;

	using ModelAsset = AssetHandle<Model>;
	using ShaderAsset = AssetHandle<Shader>;
	using TextureAsset = AssetHandle<Texture>;

	class AssetManager {
	public:
		template<typename T>
		AssetHandle<T> load(const std::string& filepath);

		template<typename T>
		AssetHandle<T> reload(const std::string& filepath);

		template<typename T>
		AssetHandle<T> getHandle(const std::string& filepath);

		template<typename T>
		bool exists(const std::string& filepath);

		template<typename T>
		void clearAssets();

		template<typename T>
		size_t numberOfAssets();

	private:
		template<typename T>
		entt::resource_cache<T>& getResourceCache();

	private:
		entt::resource_cache<Model> m_modelCache;
		entt::resource_cache<Shader> m_shaderCache;
		entt::resource_cache<Texture> m_textureCache;
	};

}