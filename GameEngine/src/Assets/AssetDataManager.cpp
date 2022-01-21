#include "AssetDataManager.h"
#include "AssetManager.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <fstream>

#include <iostream>

#define EXPLICIT_TEMPLATE_INSTANTIATION(T) \
    template AssetHandleIDtype AssetDataManager::importAsset<T>(const std::string& assetPath, ImportSettings<T> importSettings); \
    template AssetData<T>* AssetDataManager::getAssetData(AssetHandleIDtype ID, AssetHandleIDtype localID); \
    template bool AssetDataManager::exists<T>(AssetHandleIDtype ID, AssetHandleIDtype localID);

namespace GameEngine {

    std::filesystem::path getPathFromAssetFolder(const std::string& path, const std::string& assetFolderPath) {
        std::filesystem::path pathFromAssetFolder = std::filesystem::path(path).lexically_normal();
        if(pathFromAssetFolder.is_absolute()) {
            pathFromAssetFolder = pathFromAssetFolder.lexically_relative(std::filesystem::absolute(assetFolderPath));
        }
        return pathFromAssetFolder;
    }

    std::filesystem::path getAbsolutePath(const std::string& path, const std::string& assetFolderPath) {
        std::filesystem::path originalPath(path);
        if(originalPath.is_absolute()) return originalPath.lexically_normal();
        else return (std::filesystem::absolute(assetFolderPath) / originalPath).lexically_normal();
    }

    std::pair<AssetHandleIDtype, std::string> getIDAndNameFromAssetDataFile(const std::string& absoluteAssetPath) {
        using namespace std::filesystem;

        std::string assetFilename = path(absoluteAssetPath).filename();
        path assetDataPath = path(absoluteAssetPath).parent_path() / ".assetdata";
        YAML::Node assetDataRoot;
        if(std::filesystem::exists(assetDataPath)) assetDataRoot = YAML::LoadFile(assetDataPath.string());

        for(const YAML::Node& assetNode : assetDataRoot) {
            if(assetNode["filename"].as<std::string>() == assetFilename) {
                return {assetNode["id"].as<AssetHandleIDtype>(), assetNode["name"].as<std::string>()};
            }
        }
        return {0, ""};
    }

    template <typename T>
    void writeAssetData(const AssetData<T>& assetData, YAML::Emitter& emitter) {
        emitter << YAML::Key << "name" << YAML::Value << assetData.name;
    }

    template <>
    void writeAssetData<Texture>(const AssetData<Texture>& assetData, YAML::Emitter& emitter) {
        emitter << YAML::Key << "name" << YAML::Value << assetData.name;

        emitter << YAML::Key << "srgb";
        emitter << YAML::Value << assetData.srgb;
    }


    template <>
    void writeAssetData<Model>(const AssetData<Model>& assetData, YAML::Emitter& emitter) {
        emitter << YAML::Key << "name" << YAML::Value << assetData.name;

        emitter << YAML::Key << "materials";
        emitter << YAML::Value << assetData.materialIDs;
        
        if(assetData.embeddedTextures.size() > 0) {
            emitter << YAML::Key << "embeddedTextures";
            emitter << YAML::Value << YAML::BeginSeq;
            for(const auto& embeddedTexture : assetData.embeddedTextures) {
                emitter << YAML::BeginMap;
                writeAssetData<Texture>(embeddedTexture, emitter);
                emitter << YAML::EndMap;
            }
            emitter << YAML::EndSeq;
        }
    }

    template <typename T>
    AssetData<T> readAssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, const YAML::Node&) {
        return AssetData<T>(ID, localID, filepath, name);
    }

    template <>
    AssetData<Model> readAssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, const YAML::Node& node) {
        std::vector<AssetHandleIDtype> materials = node["materials"].as<std::vector<AssetHandleIDtype>>();
        return AssetData<Model>(ID, localID, filepath, name, materials);
    }

    template <>
    AssetData<Texture> readAssetData(AssetHandleIDtype ID, AssetHandleIDtype localID, const std::string& filepath, const std::string& name, const YAML::Node& node) {
        bool srgb = node["srgb"].as<bool>();
        return AssetData<Texture>(ID, localID, filepath, name, srgb);
    }

    template <> AssetDataManager::AssetDataMap<Model>& AssetDataManager::getAssetDataMap() {
        return m_modelAssetDataMap;
    }

    template <> AssetDataManager::AssetDataMap<Shader>& AssetDataManager::getAssetDataMap() {
        return m_shaderAssetDataMap;
    }

    template <> AssetDataManager::AssetDataMap<Texture>& AssetDataManager::getAssetDataMap() {
        return m_textureAssetDataMap;
    }

    template <> AssetDataManager::AssetDataMap<Material>& AssetDataManager::getAssetDataMap() {
        return m_materialAssetDataMap;
    }

    AssetDataManager::AssetDataManager(const std::string& assetFolderPath, AssetManager* assetManager)
        : m_assetFolderPath(assetFolderPath), m_assetManager(assetManager) {

        reloadAssetData();
    }

    void AssetDataManager::reloadAssetData() {
        if(!std::filesystem::exists(m_assetFolderPath)) return;
        clearRegisteredAssets();

        for(auto& dirEntry : std::filesystem::recursive_directory_iterator(m_assetFolderPath)) {
            if(dirEntry.is_regular_file() && std::filesystem::path(dirEntry).filename() == ".assetdata") {
                YAML::Node rootNode = YAML::LoadFile(std::filesystem::path(dirEntry));

                for(const YAML::Node& assetNode : rootNode) {
                    if(assetNode["id"] && assetNode["filename"]) {
                        std::string assetFilepath = std::filesystem::absolute(dirEntry).parent_path() / assetNode["filename"].as<std::string>();

                        AssetHandleIDtype id = assetNode["id"].as<AssetHandleIDtype>();
                        std::string name = assetNode["name"].as<std::string>();
                        std::string fileExtenstion = std::filesystem::path(assetFilepath).extension();

                        if(isValidFileExtensionForAssetType<Model>(fileExtenstion)) {
                            registerAssetData<Model>(readAssetData<Model>(id, 0, assetFilepath, name, assetNode));

                            AssetHandleIDtype embeddedTextureID = 1;
                            for(auto& embeddedTextureNode : assetNode["embeddedTextures"]) {
                                std::string embeddedTextureName = embeddedTextureNode["name"].as<std::string>();
                                registerAssetData<Texture>(readAssetData<Texture>(id, embeddedTextureID, assetFilepath, embeddedTextureName, embeddedTextureNode));
                                embeddedTextureID++;
                            }
                        }
                        else if(isValidFileExtensionForAssetType<Shader>(fileExtenstion)) {
                            registerAssetData<Shader>(readAssetData<Shader>(id, 0, assetFilepath, name, assetNode));
                        }
                        else if(isValidFileExtensionForAssetType<Texture>(fileExtenstion)) {
                            registerAssetData<Texture>(readAssetData<Texture>(id, 0, assetFilepath, name, assetNode));
                        }
                        else if(isValidFileExtensionForAssetType<Material>(fileExtenstion)) {
                            registerAssetData<Material>(readAssetData<Material>(id, 0, assetFilepath, name, assetNode));
                        }
                    }
                }
            }
        }
    }

    void AssetDataManager::clearRegisteredAssets() {
        getAssetDataMap<Model>().map.clear();
        getAssetDataMap<Shader>().map.clear();
        getAssetDataMap<Texture>().map.clear();
        getAssetDataMap<Material>().map.clear();
    }

    template <typename T>
    AssetHandleIDtype AssetDataManager::importAsset(const std::string& assetPath, ImportSettings<T> importSettings) {
        if(!isValidFileExtensionForAssetType<T>(std::filesystem::path(assetPath).extension())) {
            std::cout << "Could not import asset: " << assetPath << ". Invalid file extension" << std::endl;
            return 0;
        }

        // Get paths
        std::filesystem::path pathFromAssetFolder = getPathFromAssetFolder(assetPath, m_assetFolderPath);
        std::filesystem::path absolutePath = getAbsolutePath(assetPath, m_assetFolderPath);
        std::string filename = absolutePath.filename();

        // Check that the path is valid
        if(!std::filesystem::exists(absolutePath)) {
            std::cout << "Could not import asset: " << absolutePath << ". Path does not exist" << std::endl;
            return 0;
        }

        std::filesystem::path testPath = absolutePath;
        while(testPath.has_relative_path() && testPath != std::filesystem::absolute(m_assetFolderPath)) {
            testPath = testPath.parent_path();
        }
        if(testPath != std::filesystem::absolute(m_assetFolderPath)) {
            std::cout << "Could not import asset: " << absolutePath << ". Path is not a subpath of " << m_assetFolderPath << std::endl;
            return 0;
        }

        // Make AssetData, and load in the new data from file
        auto [id, name] = getIDAndNameFromAssetDataFile(absolutePath);
        if(id == 0) {
            id = nextID();
            name = absolutePath.stem();
        }

        AssetData<T> assetData(id, 0, absolutePath.string(), name, importSettings, this);

        // Load .assetdata file
        std::filesystem::path assetDataPath = absolutePath.parent_path() / ".assetdata";
        YAML::Node assetDataRoot;
        if(std::filesystem::exists(assetDataPath)) assetDataRoot = YAML::LoadFile(assetDataPath.string());

        // Rebuild .assetdata, removing the asset if it already exists
        YAML::Emitter emitter;
        emitter << YAML::BeginSeq;

        for(const YAML::Node& assetNode : assetDataRoot) {
            if(assetNode["filename"].as<std::string>() != filename) {
                emitter << assetNode;
            }
        }

        // Create entry in .assetdata for the imported asset
        emitter << YAML::BeginMap;

        emitter << YAML::Key << "id" << YAML::Value << assetData.ID;
        emitter << YAML::Key << "filename" << YAML::Value << filename;

        // Write asset-type specific data to .assetdata
        writeAssetData(assetData, emitter);

        emitter << YAML::EndMap;

        emitter << YAML::EndSeq;

        // Write to .assetdata file
        std::ofstream assetDataFile(assetDataPath.c_str(), std::ios::trunc | std::ios::out);
        if(assetDataFile.is_open()) {
            assetDataFile << emitter.c_str();
            assetDataFile.close();
        }

        // Register asset data
        registerAssetData<T>(assetData);
        
        return id;
    }

    template <typename T>
    AssetData<T>* AssetDataManager::getAssetData(AssetHandleIDtype ID, AssetHandleIDtype localID) {
        auto search = getAssetDataMap<T>().map.find(ID);
        if(search == getAssetDataMap<T>().map.end()) return nullptr;
        auto searchLocal = search->second.find(localID);
        if(searchLocal == search->second.end()) return nullptr;
        return &(searchLocal->second);
    }

    template <typename T>
    bool AssetDataManager::exists(AssetHandleIDtype ID, AssetHandleIDtype localID) {
        auto search = getAssetDataMap<T>().map.find(ID);
        return (search != getAssetDataMap<T>().map.end() && search->second.find(localID) != search->second.end());
    }

    AssetHandleIDtype AssetDataManager::nextID() {
        return m_nextID++;
    }

    template <typename T>
    void AssetDataManager::registerAssetData(const AssetData<T>& assetData) {
        getAssetDataMap<T>().map[assetData.ID].insert({assetData.localID, assetData});
        if(assetData.ID >= m_nextID) m_nextID = assetData.ID + 1;
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Model);
    EXPLICIT_TEMPLATE_INSTANTIATION(Shader);
    EXPLICIT_TEMPLATE_INSTANTIATION(Texture);
    EXPLICIT_TEMPLATE_INSTANTIATION(Material);

}
