#pragma once
#include "AssetHandle.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace GameEngine {

    template<class AssetType>
    class AssetCache {
    public:
        using IDtype = uint32_t;

    private:
        struct AssetData {
            AssetData(const std::string& name, const std::string& filepath, AssetHandle<AssetType> asset)
                : name(name), filepath(filepath), asset(asset) {}
            std::string name;
            std::string filepath;
            AssetHandle<AssetType> asset;
        };

    public:
        template <typename Loader, typename... Args>
        AssetHandle<AssetType> load(IDtype id, const std::string& name, const std::string& filepath, Args&&... args) {
            auto search = m_assets.find(id);
            if(search == m_assets.end()) {
                Loader loader;
                AssetHandle<AssetType> assetHandle(loader.load(std::forward<Args>(args)...));
                m_assets.insert({id, AssetData(name, filepath, assetHandle)});
                return assetHandle;
            }
            else {
                return search->second.asset;
            }
        }

        template <typename Loader, typename... Args>
        AssetHandle<AssetType> reload(IDtype id, std::string name, std::string filepath, Args&&... args) {
            discard(id);
            return load<Loader>(id, name, filepath, args...);
        }

        AssetHandle<AssetType> handle(IDtype id) {
            auto search = m_assets.find(id);
            if(search == m_assets.end()) return {};
            return search->second.asset;
        }

        std::size_t size() const {
            return m_assets.size();
        }

        void clear() {
            m_assets.clear();
        }

        void discard(IDtype id) {
            auto search = m_assets.find(id);
            if(search != m_assets.end()) m_assets.erase(search);
        }

        bool contains(IDtype id) const {
            return m_assets.contains(id);
        }

        template <typename Func>
        void each(Func func) const {
            auto begin = m_assets.begin();
            auto end = m_assets.end();

            while(begin != end) {
                auto curr = begin++;

                func(curr->second.asset);
            }
        }

    private:

        std::unordered_map<IDtype, AssetData> m_assets;
    };

}
