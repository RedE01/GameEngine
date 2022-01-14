#pragma once
#include "AssetData.h"
#include <string>
#include <unordered_map>
#include <functional>

namespace GameEngine {

    class AssetManager;

    class AssetDataManager {
    private:
        template<typename T>
        class AssetDataMap {
        public:
            std::unordered_map<AssetHandleIDtype, AssetData<T>> map;
        };

    public:
        AssetDataManager(const std::string& assetFolderPath, AssetManager* assetManager);

        void reloadAssetData();
        void clearRegisteredAssets();

        AssetHandleIDtype importAsset(const std::string& assetPath);

        template <typename T>
        AssetHandleIDtype importAsset(const std::string& assetPath);

        template <typename T>
        AssetData<T>* getAssetData(AssetHandleIDtype ID);

        template <typename T>
        bool exists(AssetHandleIDtype ID);

        template <typename T>
        bool isValidFileExtensionForAssetType(const std::string& ext);

        template <typename T>
        void each(std::function<void(AssetData<T>&)> func) {
            for(auto& e : getAssetDataMap<T>().map) func(e.second);
        }

        AssetHandleIDtype nextID();

    private:
        template <typename T>
        AssetDataMap<T>& getAssetDataMap();

        template <typename T>
        void registerAssetData(AssetHandleIDtype id, const AssetData<T>& assetData);

    private:
        const std::string m_assetFolderPath;
        AssetManager* m_assetManager;

        AssetDataMap<Model> m_modelAssetDataMap;
        AssetDataMap<Shader> m_shaderAssetDataMap;
        AssetDataMap<Texture> m_textureAssetDataMap;
        AssetDataMap<Material> m_materialAssetDataMap;

        AssetHandleIDtype m_nextID = 1;
    };

}
