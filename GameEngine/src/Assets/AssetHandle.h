#pragma once
#include <memory>

namespace GameEngine {

	class Model;
	class Shader;
	class Texture;
	class Material;

    template<typename AssetType>
    class AssetHandle {
    public:
        AssetHandle() : m_asset() { }

        AssetHandle(std::shared_ptr<AssetType> asset) 
            : m_asset(std::move(asset)) {
        }

        const AssetType& get() const {
            return m_asset.get();
        }

        AssetType& get() {
            return *m_asset;
        }

        std::size_t use_count() const {
            return *m_asset;
        }

        operator const AssetType& () const {
            return get();
        }

        operator AssetType& () {
            return get();
        }

        const AssetType& operator*() const {
            return get();
        }

        AssetType& operator*() {
            return get();
        }

        const AssetType* operator->() const {
            return m_asset.get();
        }

        AssetType* operator->() {
            return m_asset.get();
        }

        explicit operator bool() const {
            return static_cast<bool>(m_asset);
        }
        
    private:
        std::shared_ptr<AssetType> m_asset;
    };

	using ModelAsset = AssetHandle<Model>;
	using ShaderAsset = AssetHandle<Shader>;
	using TextureAsset = AssetHandle<Texture>;
	using MaterialAsset = AssetHandle<Material>;

}
