#include "AssetManager.h"
#include "AssetDataManager.h"
#include "ModelLoader.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include <entt/core/hashed_string.hpp>
#include <filesystem>

#define EXPLICIT_TEMPLATE_INSTANTIATION(T) \
    template AssetHandle<T> AssetManager::load(AssetHandleIDtype id); \
    template AssetHandle<T> AssetManager::reload(AssetHandleIDtype id); \
    template AssetHandle<T> AssetManager::getHandle<T>(AssetHandleIDtype id); \
    template bool AssetManager::exists<T>(AssetHandleIDtype id); \
    template const char* AssetManager::getName(AssetHandle<T> asset); \
    template const char* AssetManager::getFilepath(AssetHandle<T> asset); \
    template void AssetManager::clearAssets<T>(); \
    template size_t AssetManager::numberOfAssets<T>(); \
    template void AssetManager::each(std::function<void(AssetHandle<T>)> func); \
    template AssetHandleIDtype AssetManager::import<T>(const std::string&);

namespace GameEngine {

    std::string getNameFromFilepath(const std::string& filepath);

    AssetManager::AssetManager(const std::string& assetFolderPath) {
        m_assetDataManager = std::make_unique<AssetDataManager>(assetFolderPath, this);
    }

    AssetManager::~AssetManager() {

    }

    template<typename T, typename... Args>
    AssetHandle<T> AssetManager::load(AssetHandleIDtype id, Args... args) {
        AssetData<T>* assetData = m_assetDataManager->getAssetData<T>(id);
        if(assetData) return loadInternal(id, assetData, args...);
        return AssetHandle<T>();
    }

    template<typename T, typename... Args>
    AssetHandle<T> AssetManager::reload(AssetHandleIDtype id, Args... args) {
        AssetData<T>* assetData = m_assetDataManager->getAssetData<T>(id);
        if(assetData) return reloadInternal(id, assetData, args...);
        return AssetHandle<T>();
    }

    template<typename T>
    AssetHandle<T> AssetManager::getHandle(AssetHandleIDtype id) {
        return getResourceCache<T>()->getHandle(id);
    }

    template<typename T>
    bool AssetManager::exists(AssetHandleIDtype id) {
        return getResourceCache<T>()->contains(id);
    }

    template<typename T>
    const char* AssetManager::getName(AssetHandle<T> asset) {
        AssetData<T>* assetData = m_assetDataManager->getAssetData<T>(asset.ID());
        if(assetData) return assetData->name.c_str();
        return "";
    }

    template<typename T>
    const char* AssetManager::getFilepath(AssetHandle<T> asset) {
        AssetData<T>* assetData = m_assetDataManager->getAssetData<T>(asset.ID());
        if(assetData) return assetData->filepath.c_str();
        return "";
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

    template <typename T>
    AssetHandleIDtype AssetManager::import(const std::string& filepath) {
        return m_assetDataManager->importAsset<T>(filepath);
    }

    std::string getNameFromFilepath(const std::string& filepath) {
        return std::filesystem::path(filepath).stem();
    }

    // Model
    template<>
    AssetCache<Model>* AssetManager::getResourceCache() {
        return &m_modelCache;
    }

    template<>
    AssetHandle<Model> AssetManager::loadInternal(AssetHandleIDtype id, AssetData<Model>* assetData) {
        return getResourceCache<Model>()->load<ModelLoader>(id, assetData, this);
    }

    template<>
    AssetHandle<Model> AssetManager::reloadInternal(AssetHandleIDtype id, AssetData<Model>* assetData) {
        return getResourceCache<Model>()->reload<ModelLoader>(id, assetData, this);
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Model)

    // Shader
    template<>
    AssetCache<Shader>* AssetManager::getResourceCache() {
        return &m_shaderCache;
    }

    template<>
    AssetHandle<Shader> AssetManager::loadInternal(AssetHandleIDtype id, AssetData<Shader>* assetData) {
        return getResourceCache<Shader>()->load<ShaderLoader>(id, assetData);
    }

    template<>
    AssetHandle<Shader> AssetManager::reloadInternal(AssetHandleIDtype id, AssetData<Shader>* assetData) {
        return getResourceCache<Shader>()->reload<ShaderLoader>(id, assetData);
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Shader)

    // Texture
    template<>
    AssetCache<Texture>* AssetManager::getResourceCache() {
        return &m_textureCache;
    }

    template<>
    AssetHandle<Texture> AssetManager::loadInternal(AssetHandleIDtype id, AssetData<Texture>* assetData) {
        return getResourceCache<Texture>()->load<TextureLoader>(id, assetData);
    }

    template<>
    AssetHandle<Texture> AssetManager::reloadInternal(AssetHandleIDtype id, AssetData<Texture>* assetData) {
        return getResourceCache<Texture>()->reload<TextureLoader>(id, assetData);
    }

    template<>
    AssetHandle<Texture> AssetManager::loadInternal(AssetHandleIDtype id, AssetData<Texture>* assetData, unsigned char* data, unsigned int dataLength) {
        return getResourceCache<Texture>()->load<TextureLoader>(id, assetData, data, dataLength);
    }

    template<>
    AssetHandle<Texture> AssetManager::reloadInternal(AssetHandleIDtype id, AssetData<Texture>* assetData, unsigned char* data, unsigned int dataLength) {
        return getResourceCache<Texture>()->reload<TextureLoader>(id, assetData, data, dataLength);
    }

    template AssetHandle<Texture> AssetManager::load(AssetHandleIDtype id, unsigned char* data, unsigned int dataLength);
    template AssetHandle<Texture> AssetManager::reload(AssetHandleIDtype id, unsigned char* data, unsigned int dataLength);
    EXPLICIT_TEMPLATE_INSTANTIATION(Texture)

    // Material
    template<>
    AssetCache<Material>* AssetManager::getResourceCache() {
        return &m_materialCache;
    }

    template<>
    AssetHandle<Material> AssetManager::loadInternal(AssetHandleIDtype id, AssetData<Material>* assetData) {
        return getResourceCache<Material>()->load<MaterialLoader>(id, assetData, this);
    }

    template<>
    AssetHandle<Material> AssetManager::reloadInternal(AssetHandleIDtype id, AssetData<Material>* assetData) {
        return getResourceCache<Material>()->reload<MaterialLoader>(id, assetData, this);
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Material)

	// ----

}
