#pragma once
#include "ModelLoader.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include "AssetHandle.h"
#include <entt/resource/cache.hpp>

namespace GameEngine {

	class AssetManager {
	public:
		template<typename T, typename ... Args>
		AssetHandle<T> load(const std::string& filepath, Args...);

		template<typename T, typename ... Args>
		AssetHandle<T> reload(const std::string& filepath, Args...);

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
		entt::resource_cache<Material> m_materialCache;
	};

}