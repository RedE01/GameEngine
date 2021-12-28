#include "AssetManager.h"
#include "ModelLoader.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include <entt/core/hashed_string.hpp>

namespace GameEngine {

    template<typename T, typename ... Args>
    AssetHandle<T> AssetManager::load(const std::string&, Args...) {
        // Error: T does not have a loader
        assert(0);
        return AssetHandle<T>();
    }

    template<typename T, typename ... Args>
    AssetHandle<T> AssetManager::reload(const std::string&, Args...) {
        // Error: T does not have a loader
        assert(0);
        return AssetHandle<T>();
    }

    template<typename T>
    AssetHandle<T> AssetManager::getHandle(const std::string& filepath) {
        return getResourceCache<T>()->handle(entt::hashed_string{filepath.c_str()});
    }

    template<typename T>
    bool AssetManager::exists(const std::string& filepath) {
        return getResourceCache<T>()->contains(entt::hashed_string{filepath.c_str()});
    }

    template<typename T>
    void AssetManager::clearAssets() {
        getResourceCache<T>()->clear();
    }

    template<typename T>
    size_t AssetManager::numberOfAssets() {
        return getResourceCache<T>()->size();
    }

    template<typename T>
    void AssetManager::each(std::function<void(AssetHandle<T>)> func) {
        return getResourceCache<T>()->each(func);
    }

    template<typename T>
    AssetCache<T>* AssetManager::getResourceCache() {
        assert(false);
        return nullptr;
    }

    template<typename T, typename Loader, typename... Args>
    AssetHandle<T> AssetManager::loadInternal(const std::string& filepath, Args... args) {
        return getResourceCache<T>()->template load<Loader>(entt::hashed_string{filepath.c_str()}, filepath, filepath, args...);
    }

    template<typename T, typename Loader, typename... Args>
    AssetHandle<T> AssetManager::reloadInternal(const std::string& filepath, Args... args) {
        return getResourceCache<T>()->template reload<Loader>(entt::hashed_string{filepath.c_str()}, filepath, filepath, args...);
    }

    // Model
    template<>
    AssetCache<Model>* AssetManager::getResourceCache() {
        return &m_modelCache;
    }

    template<>
    AssetHandle<Model> AssetManager::load<Model>(const std::string& filepath) {
        return loadInternal<Model, ModelLoader>(filepath, filepath, this);
    }

    template<>
    AssetHandle<Model> AssetManager::reload<Model>(const std::string& filepath) {
        return reloadInternal<Model, ModelLoader>(filepath, filepath, this);
    }

    template AssetHandle<Model> AssetManager::getHandle<Model>(const std::string& filepath);
    template bool AssetManager::exists<Model>(const std::string& filepath);
    template void AssetManager::clearAssets<Model>();
    template size_t AssetManager::numberOfAssets<Model>();
    template void AssetManager::each(std::function<void(AssetHandle<Model>)> func);

    // Shader
    template<>
    AssetCache<Shader>* AssetManager::getResourceCache() {
        return &m_shaderCache;
    }

    template<>
    AssetHandle<Shader> AssetManager::load<Shader>(const std::string& filepath) {
        return loadInternal<Shader, ShaderLoader>(filepath, filepath);
    }

    template<>
    AssetHandle<Shader> AssetManager::reload<Shader>(const std::string& filepath) {
        return reloadInternal<Shader, ShaderLoader>(filepath, filepath);
    }

    template AssetHandle<Shader> AssetManager::getHandle<Shader>(const std::string& filepath);
    template bool AssetManager::exists<Shader>(const std::string& filepath);
    template void AssetManager::clearAssets<Shader>();
    template size_t AssetManager::numberOfAssets<Shader>();
    template void AssetManager::each(std::function<void(AssetHandle<Shader>)> func);

    // Texture
    template<>
    AssetCache<Texture>* AssetManager::getResourceCache() {
        return &m_textureCache;
    }

    template<>
    AssetHandle<Texture> AssetManager::load<Texture>(const std::string& filepath, bool srgb) {
        return loadInternal<Texture, TextureLoader>(filepath, filepath, srgb);
    }

    template<>
    AssetHandle<Texture> AssetManager::reload<Texture>(const std::string& filepath, bool srgb) {
        return reloadInternal<Texture, TextureLoader>(filepath, filepath, srgb);
    }

    template<>
    AssetHandle<Texture> AssetManager::load<Texture>(const std::string& identifier, unsigned char* data, unsigned int dataLength, bool srgb) {
        return loadInternal<Texture, TextureLoader>(identifier, identifier, data, dataLength, srgb);
    }

    template<>
    AssetHandle<Texture> AssetManager::reload<Texture>(const std::string& identifier, unsigned char* data, unsigned int dataLength, bool srgb) {
        return reloadInternal<Texture, TextureLoader>(identifier, identifier, data, dataLength, srgb);
    }

    template AssetHandle<Texture> AssetManager::getHandle<Texture>(const std::string& filepath);
    template bool AssetManager::exists<Texture>(const std::string& filepath);
    template void AssetManager::clearAssets<Texture>();
    template size_t AssetManager::numberOfAssets<Texture>();
    template void AssetManager::each(std::function<void(AssetHandle<Texture>)> func);

    // Material
    template<>
    AssetCache<Material>* AssetManager::getResourceCache() {
        return &m_materialCache;
    }

    template<>
    AssetHandle<Material> AssetManager::load<Material>(const std::string& filepath) {
        return loadInternal<Material, MaterialLoader>(filepath);
    }

    template<>
    AssetHandle<Material> AssetManager::reload<Material>(const std::string& filepath) {
        return reloadInternal<Material, MaterialLoader>(filepath);
    }

    template AssetHandle<Material> AssetManager::getHandle<Material>(const std::string& filepath);
    template bool AssetManager::exists<Material>(const std::string& filepath);
    template void AssetManager::clearAssets<Material>();
    template size_t AssetManager::numberOfAssets<Material>();
    template void AssetManager::each(std::function<void(AssetHandle<Material>)> func);

	// ----

}
