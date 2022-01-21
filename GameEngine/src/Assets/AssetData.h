#pragma once
#include "AssetHandle.h"
#include <vector>

namespace GameEngine {

    class AssetDataManager;

    template <typename T>
    bool isValidFileExtensionForAssetType(const std::string& ext);

    template <typename T>
    struct ImportSettings {
    };

    template <>
    struct ImportSettings<Model> {
        ImportSettings(bool importMaterials, const std::string& materialBaseDir)
            : importMaterials(importMaterials), materialBaseDir(materialBaseDir) {}
        const bool importMaterials;
        const std::string materialBaseDir;
    };

    template <>
    struct ImportSettings<Texture> {
        ImportSettings(bool srgb)
            : srgb(srgb) {}
        const bool srgb;
    };

    class AssetDataBase {
    public:
        AssetDataBase(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string name)
            : ID(ID), localID(localID), filepath(filepath), name(name) {
        }

    public:
        const AssetHandleIDtype ID;
        const AssetHandleIDtype localID;
        const std::string filepath;
        std::string name;
    };

    template <typename T>
    class AssetData : public AssetDataBase {
    };

    template <>
    class AssetData<Model> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, const std::vector<AssetHandleIDtype>& materialIDs) 
            : AssetDataBase(ID, localID, filepath, name), materialIDs(materialIDs) {}
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, ImportSettings<Model> importSettings, AssetDataManager* assetDataManager);

    public:
        std::vector<AssetHandleIDtype> materialIDs;
        std::vector<AssetData<Texture>> embeddedTextures;
    };

    template <>
    class AssetData<Shader> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name)
            : AssetDataBase(ID, localID, filepath, name) {}
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, ImportSettings<Shader>, AssetDataManager*)
            : AssetDataBase(ID, localID, filepath, name) {}
    };

    template <>
    class AssetData<Texture> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, bool srgb)
            : AssetDataBase(ID, localID, filepath, name), srgb(srgb) {}
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, ImportSettings<Texture>, AssetDataManager*)
            : AssetDataBase(ID, localID, filepath, name), srgb(true) {}

    public:
        bool srgb;
    };

    template <>
    class AssetData<Material> : public AssetDataBase {
    public:
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name)
            : AssetDataBase(ID, localID, filepath, name) {}
        AssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, ImportSettings<Material>, AssetDataManager*)
            : AssetDataBase(ID, localID, filepath, name) {}
    };

}
