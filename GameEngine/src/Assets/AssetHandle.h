#pragma once
#include <memory>
#include <variant>

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
        AssetHandle() : m_id(0), m_localID(0), m_asset() { }

        AssetHandle(IDtype id, IDtype localID, std::shared_ptr<AssetType> asset) 
            : m_id(id), m_localID(localID), m_asset(std::move(asset)) {
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

        IDtype localID() {
            return m_localID;
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
            return m_asset.get() != nullptr;
        }
        
    private:
        IDtype m_id;
        IDtype m_localID;
        std::shared_ptr<AssetType> m_asset;
    };

	using ModelAsset = AssetHandle<Model>;
	using ShaderAsset = AssetHandle<Shader>;
	using TextureAsset = AssetHandle<Texture>;
	using MaterialAsset = AssetHandle<Material>;

    using AssetVariant = std::variant<ModelAsset, ShaderAsset, TextureAsset, MaterialAsset>;
}
