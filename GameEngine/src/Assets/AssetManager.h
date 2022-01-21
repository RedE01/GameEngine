#pragma once
#include "AssetHandle.h"
#include "AssetCache.h"
#include "AssetData.h"
#include <functional>

namespace GameEngine {

    class AssetDataManager;

    template <typename T>
    class AssetData;

	class AssetManager {
	public:
        AssetManager(const std::string& assetFolderPath);
        ~AssetManager();

        template<typename T>
        AssetHandle<T> load(AssetHandleIDtype id, AssetHandleIDtype localID);

        template<typename T, typename... Args>
        AssetHandle<T> load(AssetHandleIDtype id, AssetHandleIDtype localID, Args... args);

        template<typename T>
        AssetHandle<T> getHandle(AssetHandleIDtype id, AssetHandleIDtype localID = 0);

        template<typename T>
        bool exists(AssetHandleIDtype id, AssetHandleIDtype localID = 0);

        template<typename T>
        const char* getName(AssetHandle<T> asset);

        template<typename T>
        const char* getFilepath(AssetHandle<T> asset);

        template<typename T>
        void clearAssets();

        template<typename T>
        size_t numberOfAssets();

        template<typename T>
        void each(std::function<void(AssetHandle<T>)> func);

        template<typename T>
        void each(std::function<void(AssetData<T>&)> func);

        template <typename T>
        AssetHandleIDtype import(const std::string& filepath, ImportSettings<T> importSettings);

	private:
        template<typename T>
        AssetCache<T>* getResourceCache();

        void loadAny(AssetHandleIDtype id);

        template<typename T, typename... Args>
        AssetHandle<T> loadInternal(AssetHandleIDtype id, AssetHandleIDtype localID, AssetData<T>* assetData, Args... args);

	private:
        std::unique_ptr<AssetDataManager> m_assetDataManager;
		AssetCache<Model> m_modelCache;
		AssetCache<Shader> m_shaderCache;
		AssetCache<Texture> m_textureCache;
		AssetCache<Material> m_materialCache;
	};

}
