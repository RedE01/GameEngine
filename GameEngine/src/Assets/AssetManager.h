#pragma once
#include "AssetHandle.h"
#include "AssetCache.h"
#include <functional>

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

        template<typename T>
        void each(std::function<void(AssetHandle<T>)> func);

        template<typename T>
        void each(std::function<void(AssetHandle<T>, const std::string& name, const std::string& filename)> func);

	private:
        template<typename T>
        AssetCache<T>* getResourceCache();
        
        template<typename T, typename Loader, typename... Args>
        AssetHandle<T> loadInternal(const std::string& filepath, Args... args);

        template<typename T, typename Loader, typename... Args>
        AssetHandle<T> reloadInternal(const std::string& filepath, Args... args);

	private:
		AssetCache<Model> m_modelCache;
		AssetCache<Shader> m_shaderCache;
		AssetCache<Texture> m_textureCache;
		AssetCache<Material> m_materialCache;
	};

}
