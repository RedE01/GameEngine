#include "AssetManager.h"
#include <entt/core/hashed_string.hpp>

namespace GameEngine {

	template<typename T, typename ... Args>
	AssetHandle<T> AssetManager::load(const std::string& filepath, Args...) {
		// Error: T does not have a loader
		assert(0);
		return AssetHandle<T>();
	}

	template<typename T, typename ... Args>
	AssetHandle<T> AssetManager::reload(const std::string& filepath, Args...) {
		// Error: T does not have a loader
		assert(0);
		return AssetHandle<T>();
	}

	template<typename T>
	AssetHandle<T> AssetManager::getHandle(const std::string& filepath) {
		return getResourceCache<T>().handle(entt::hashed_string{filepath.c_str()});
	}

	template<typename T>
	bool AssetManager::exists(const std::string& filepath) {
		return getResourceCache<T>().contains(entt::hashed_string{filepath.c_str()});
	}

	template<typename T>
	void AssetManager::clearAssets() {
		getResourceCache<T>().clear();
	}

	template<typename T>
	size_t AssetManager::numberOfAssets() {
		return getResourceCache<T>().size();
	}

	template<typename T>
	entt::resource_cache<T>& AssetManager::getResourceCache() {
		assert(false);
		return 0;
	}

	// Model
	template<>
	entt::resource_cache<Model>& AssetManager::getResourceCache() {
		return m_modelCache;
	}

	template<>
	AssetHandle<Model> AssetManager::load<Model>(const std::string& filepath) {
		return getResourceCache<Model>().load<ModelLoader>(entt::hashed_string{filepath.c_str()}, filepath, this);
	}

	template<>
	AssetHandle<Model> AssetManager::reload<Model>(const std::string& filepath) {
		return getResourceCache<Model>().reload<ModelLoader>(entt::hashed_string{filepath.c_str()}, filepath, this);
	}

	template AssetHandle<Model> AssetManager::getHandle<Model>(const std::string& filepath);
	template bool AssetManager::exists<Model>(const std::string& filepath);
	template void AssetManager::clearAssets<Model>();
	template size_t AssetManager::numberOfAssets<Model>();

	// Shader
	template<>
	entt::resource_cache<Shader>& AssetManager::getResourceCache() {
		return m_shaderCache;
	}

	template<>
	AssetHandle<Shader> AssetManager::load<Shader>(const std::string& filepath) {
		return getResourceCache<Shader>().load<ShaderLoader>(entt::hashed_string{filepath.c_str()}, filepath);
	}

	template<>
	AssetHandle<Shader> AssetManager::reload<Shader>(const std::string& filepath) {
		return getResourceCache<Shader>().reload<ShaderLoader>(entt::hashed_string{filepath.c_str()}, filepath);
	}

	template AssetHandle<Shader> AssetManager::getHandle<Shader>(const std::string& filepath);
	template bool AssetManager::exists<Shader>(const std::string& filepath);
	template void AssetManager::clearAssets<Shader>();
	template size_t AssetManager::numberOfAssets<Shader>();

	// Texture
	template<>
	entt::resource_cache<Texture>& AssetManager::getResourceCache() {
		return m_textureCache;
	}

	template<>
	AssetHandle<Texture> AssetManager::load<Texture>(const std::string& filepath) {
		return getResourceCache<Texture>().load<TextureLoader>(entt::hashed_string{filepath.c_str()}, filepath);
	}

	template<>
	AssetHandle<Texture> AssetManager::reload<Texture>(const std::string& filepath) {
		return getResourceCache<Texture>().reload<TextureLoader>(entt::hashed_string{filepath.c_str()}, filepath);
	}

	template<>
	AssetHandle<Texture> AssetManager::load<Texture>(const std::string& identifier, unsigned char* data, unsigned int dataLength) {
		return getResourceCache<Texture>().load<TextureLoader>(entt::hashed_string{identifier.c_str()}, identifier, data, dataLength);
	}

	template<>
	AssetHandle<Texture> AssetManager::reload<Texture>(const std::string& identifier, unsigned char* data, unsigned int dataLength) {
		return getResourceCache<Texture>().reload<TextureLoader>(entt::hashed_string{identifier.c_str()}, identifier, data, dataLength);
	}

	template AssetHandle<Texture> AssetManager::getHandle<Texture>(const std::string& filepath);
	template bool AssetManager::exists<Texture>(const std::string& filepath);
	template void AssetManager::clearAssets<Texture>();
	template size_t AssetManager::numberOfAssets<Texture>();

	// Material
	template<>
	entt::resource_cache<Material>& AssetManager::getResourceCache() {
		return m_materialCache;
	}

	template<>
	AssetHandle<Material> AssetManager::load<Material>(const std::string& filepath) {
		return getResourceCache<Material>().load<MaterialLoader>(entt::hashed_string{filepath.c_str()});
	}

	template<>
	AssetHandle<Material> AssetManager::reload<Material>(const std::string& filepath) {
		return getResourceCache<Material>().reload<MaterialLoader>(entt::hashed_string{filepath.c_str()});
	}

	template AssetHandle<Material> AssetManager::getHandle<Material>(const std::string& filepath);
	template bool AssetManager::exists<Material>(const std::string& filepath);
	template void AssetManager::clearAssets<Material>();
	template size_t AssetManager::numberOfAssets<Material>();

	// ----

}