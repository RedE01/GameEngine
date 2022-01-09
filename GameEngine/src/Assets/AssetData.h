#pragma once
#include "AssetHandle.h"
#include <vector>

namespace GameEngine {

    class AssetDataManager;

    class AssetDataBase {
    public:
        AssetDataBase(AssetHandleIDtype ID, const std::string& filepath, const std::string name)
            : ID(ID), filepath(filepath), name(name) {
        }

    public:
        const AssetHandleIDtype ID;
        const std::string filepath;
        std::string name;
    };

    template <typename T>
    class AssetData : public AssetDataBase {
    };

    template <>
    class AssetData<Model> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name, const std::vector<AssetHandleIDtype>& materialIDs) 
            : AssetDataBase(ID, filepath, name), materialIDs(materialIDs) {}
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name, AssetDataManager* assetDataManager);

    public:
        std::vector<AssetHandleIDtype> materialIDs;
    };

    template <>
    class AssetData<Shader> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name)
            : AssetDataBase(ID, filepath, name) {}
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name, AssetDataManager*)
            : AssetDataBase(ID, filepath, name) {}
    };

    template <>
    class AssetData<Texture> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name, bool srgb)
            : AssetDataBase(ID, filepath, name), srgb(srgb) {}
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name, AssetDataManager*)
            : AssetDataBase(ID, filepath, name), srgb(true) {}

    public:
        bool srgb;
    };

    template <>
    class AssetData<Material> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name)
            : AssetDataBase(ID, filepath, name) {}
        AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name, AssetDataManager*)
            : AssetDataBase(ID, filepath, name) {}
    };

}
