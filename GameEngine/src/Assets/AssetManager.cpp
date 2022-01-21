#include "AssetManager.h"
#include "AssetDataManager.h"
#include "ModelLoader.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include <entt/core/hashed_string.hpp>
#include <filesystem>

#define EXPLICIT_TEMPLATE_INSTANTIATION(T) \
    template AssetHandle<T> AssetManager::load(AssetHandleIDtype id, AssetHandleIDtype localID); \
    template AssetHandle<T> AssetManager::getHandle<T>(AssetHandleIDtype id, AssetHandleIDtype localID); \
    template bool AssetManager::exists<T>(AssetHandleIDtype id, AssetHandleIDtype localID); \
    template const char* AssetManager::getName(AssetHandle<T> asset); \
    template const char* AssetManager::getFilepath(AssetHandle<T> asset); \
    template void AssetManager::clearAssets<T>(); \
    template size_t AssetManager::numberOfAssets<T>(); \
    template void AssetManager::each(std::function<void(AssetHandle<T>)> func); \
    template void AssetManager::each(std::function<void(AssetData<T>&)> func); \
    template AssetHandleIDtype AssetManager::import<T>(const std::string&, ImportSettings<T> importSettings);

namespace GameEngine {

    std::string getNameFromFilepath(const std::string& filepath);

    AssetManager::AssetManager(const std::string& assetFolderPath) {
        m_assetDataManager = std::make_unique<AssetDataManager>(assetFolderPath, this);
    }

    AssetManager::~AssetManager() {

    }

    template<typename T>
    AssetHandle<T> AssetManager::load(AssetHandleIDtype id, AssetHandleIDtype localID) {
        //std::cout << "Load no args: " << id << ", " << localID << std::endl;
        if(localID == 0) {
            AssetData<T>* assetData = m_assetDataManager->getAssetData<T>(id, 0);
            if(assetData) {
                return loadInternal(id, localID, assetData);
            }
            return AssetHandle<T>();
        }
        else {
            if(getHandle<T>(id, localID)) return getHandle<T>(id, localID);
            else {
                loadAny(id);
                return getHandle<T>(id, localID);
            }
        }

    }

    template<typename T, typename... Args>
    AssetHandle<T> AssetManager::load(AssetHandleIDtype id, AssetHandleIDtype localID, Args... args) {
        //std::cout << "Load with args: " << id << ", " << localID << std::endl;
        AssetData<T>* assetData = m_assetDataManager->getAssetData<T>(id, localID);
        if(assetData) {
            return loadInternal(id, localID, assetData, args...);
        }
        return AssetHandle<T>();
    }

    template<typename T>
    AssetHandle<T> AssetManager::getHandle(AssetHandleIDtype id, AssetHandleIDtype localID) {
        return getResourceCache<T>()->getHandle(id, localID);
    }

    template<typename T>
    bool AssetManager::exists(AssetHandleIDtype id, AssetHandleIDtype localID) {
        return getResourceCache<T>()->contains(id, localID);
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

    template<typename T>
    void AssetManager::each(std::function<void(AssetData<T>&)> func) {
        return m_assetDataManager->each<T>(func);
    }

    template <typename T>
    AssetHandleIDtype AssetManager::import(const std::string& filepath, ImportSettings<T> importSettings) {
        return m_assetDataManager->importAsset<T>(filepath, importSettings);
    }

    void AssetManager::loadAny(AssetHandleIDtype id) {
        load<Model>(id, 0);
        load<Shader>(id, 0);
        load<Texture>(id, 0);
        load<Material>(id, 0);
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
    AssetHandle<Model> AssetManager::loadInternal(AssetHandleIDtype id, AssetHandleIDtype localID, AssetData<Model>* assetData) {
        return getResourceCache<Model>()->load<ModelLoader>(id, localID, assetData, this);
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Model)

    // Shader
    template<>
    AssetCache<Shader>* AssetManager::getResourceCache() {
        return &m_shaderCache;
    }

    template<>
    AssetHandle<Shader> AssetManager::loadInternal(AssetHandleIDtype id, AssetHandleIDtype localID, AssetData<Shader>* assetData) {
        return getResourceCache<Shader>()->load<ShaderLoader>(id, localID, assetData);
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Shader)

    // Texture
    template<>
    AssetCache<Texture>* AssetManager::getResourceCache() {
        return &m_textureCache;
    }

    template<>
    AssetHandle<Texture> AssetManager::loadInternal(AssetHandleIDtype id, AssetHandleIDtype localID, AssetData<Texture>* assetData) {
        return getResourceCache<Texture>()->load<TextureLoader>(id, localID, assetData);
    }

    template<>
    AssetHandle<Texture> AssetManager::loadInternal(AssetHandleIDtype id, AssetHandleIDtype localID, AssetData<Texture>* assetData, unsigned char* data, unsigned int dataLength) {
        return getResourceCache<Texture>()->load<TextureLoader>(id, localID, assetData, data, dataLength);
    }

    template AssetHandle<Texture> AssetManager::load(AssetHandleIDtype id, AssetHandleIDtype localID, unsigned char* data, unsigned int dataLength);
    EXPLICIT_TEMPLATE_INSTANTIATION(Texture)

    // Material
    template<>
    AssetCache<Material>* AssetManager::getResourceCache() {
        return &m_materialCache;
    }

    template<>
    AssetHandle<Material> AssetManager::loadInternal(AssetHandleIDtype id, AssetHandleIDtype localID, AssetData<Material>* assetData) {
        return getResourceCache<Material>()->load<MaterialLoader>(id, localID, assetData, this);
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Material)

	// ----

}
