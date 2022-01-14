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
        AssetHandle<AssetType> load(IDtype id, IDtype localID, Args&&... args) {
            auto search = m_assets.find(id);
            if(search != m_assets.end()) {
                auto searchLocal = search->second.find(localID);
                if(searchLocal != search->second.end()) return searchLocal->second;
            }

            Loader loader;
            m_assets[id].insert({localID, AssetHandle<AssetType>()}); // Insert temporary, so that we don't get stuck in an infinite loop if loader.load attempts to load this resource again
            AssetHandle<AssetType> assetHandle(id, localID, loader.load(std::forward<Args>(args)...));
            m_assets[id].erase(localID);
            if(!assetHandle) {
                return AssetHandle<AssetType>();
            }

            m_assets[id].insert({localID, assetHandle});
            return assetHandle;
        }

        AssetHandle<AssetType> getHandle(IDtype id, IDtype localID) {
            auto search = m_assets.find(id);
            if(search == m_assets.end()) return {};
            auto searchLocal = search->second.find(localID);
            if(searchLocal == search->second.end()) return {};
            return searchLocal->second;
        }

        std::size_t size() const {
            return m_assets.size();
        }

        void clear() {
            m_assets.clear();
        }

        void discard(IDtype id, IDtype localID) {
            auto search = m_assets.find(id);
            if(search != m_assets.end()) {
                auto searchLocal = search->second.find(localID);
                if(searchLocal != search->second.end()) search->second.erase(searchLocal);
                
                if(search->second.size() == 0) m_assets.erase(search);
            }
        }

        bool contains(IDtype id, IDtype localID) const {
            auto search = m_assets.find(id);
            if(search == m_assets.end() || search->second.find(localID) == search->second.end()) return false;
            return true;
        }

        void each(std::function<void(AssetHandle<AssetType>)> func) const {
            auto begin = m_assets.begin();
            auto end = m_assets.end();

            while(begin != end) {
                auto curr = begin++;

                for(auto& assetPair : curr->second) {
                    func(assetPair.second);
                }
            }
        }

    private:
        std::unordered_map<IDtype, std::unordered_map<IDtype, AssetHandle<AssetType>>> m_assets;
    };

}
