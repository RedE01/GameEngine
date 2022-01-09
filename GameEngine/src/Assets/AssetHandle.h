#pragma once
#include <memory>

namespace GameEngine {

	class Model;
	class Shader;
	class Texture;
	class Material;

    using AssetHandleIDtype = uint32_t;

    template<typename AssetType>
    class AssetHandle {
    public:
        using IDtype = AssetHandleIDtype;

    public:
        AssetHandle() : m_id(0), m_asset() { }

        AssetHandle(IDtype id, std::shared_ptr<AssetType> asset) 
            : m_id(id), m_asset(std::move(asset)) {
        }

        const AssetType& get() const {
            return m_asset.get();
        }

        AssetType& get() {
            return *m_asset;
        }

        IDtype ID() {
            return m_id;
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
        IDtype m_id;
        std::shared_ptr<AssetType> m_asset;
    };

	using ModelAsset = AssetHandle<Model>;
	using ShaderAsset = AssetHandle<Shader>;
	using TextureAsset = AssetHandle<Texture>;
	using MaterialAsset = AssetHandle<Material>;

}
