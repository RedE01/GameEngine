#pragma once
#include "AssetHandle.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

namespace GameEngine {

    template<class AssetType>
    class AssetCache {
    public:
        using IDtype = typename AssetHandle<AssetType>::IDtype;

    public:
        template <typename Loader, typename... Args>
        AssetHandle<AssetType> load(IDtype id, Args&&... args) {
            auto search = m_assets.find(id);
            if(search != m_assets.end()) return search->second;

            Loader loader;
            AssetHandle<AssetType> assetHandle(id, loader.load(std::forward<Args>(args)...));
            if(!assetHandle) return AssetHandle<AssetType>();

            m_assets.insert({id, assetHandle});
            return assetHandle;
        }

        template <typename Loader, typename... Args>
        AssetHandle<AssetType> reload(IDtype id, Args&&... args) {
            discard(id);
            return load<Loader>(id, args...);
        }

        AssetHandle<AssetType> getHandle(IDtype id) {
            auto search = m_assets.find(id);
            if(search == m_assets.end()) return {};
            return search->second;
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

        void each(std::function<void(AssetHandle<AssetType>)> func) const {
            auto begin = m_assets.begin();
            auto end = m_assets.end();

            while(begin != end) {
                auto curr = begin++;

                func(curr->second);
            }
        }

    private:
        std::unordered_map<IDtype, AssetHandle<AssetType>> m_assets;
    };

}
