#include "AssetDataManager.h"
#include "AssetManager.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <fstream>

#include <iostream>

#define EXPLICIT_TEMPLATE_INSTANTIATION(T) \
    template AssetHandleIDtype AssetDataManager::importAsset<T>(const std::string& assetPath); \
    template AssetData<T>* AssetDataManager::getAssetData(AssetHandleIDtype ID); \
    template bool AssetDataManager::exists<T>(AssetHandleIDtype ID);

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
    void writeAssetData(AssetData<T>&, YAML::Emitter&) {
    }

    template <>
    void writeAssetData<Model>(AssetData<Model>& assetData, YAML::Emitter& emitter) {
        emitter << YAML::Key << "materials";
        emitter << YAML::Value << assetData.materialIDs;
    }

    template <>
    void writeAssetData<Texture>(AssetData<Texture>& assetData, YAML::Emitter& emitter) {
        emitter << YAML::Key << "srgb";
        emitter << YAML::Value << assetData.srgb;
    }

    template<typename T>
    AssetHandleIDtype AssetDataManager::AssetDataMap<T>::nextID() {
        while(map.find(m_nextID) != map.end()) m_nextID++;
        return m_nextID;
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

    template <typename T>
    bool AssetDataManager::isValidFileExtensionForAssetType(const std::string& ext) {
        return false;
    }

    template <>
    bool AssetDataManager::isValidFileExtensionForAssetType<Model>(const std::string& ext) {
        return (ext == ".blend" || ext == ".fbx" || ext == ".gltf" || ext == ".glb" || ext == ".obj");
    }

    template <>
    bool AssetDataManager::isValidFileExtensionForAssetType<Shader>(const std::string& ext) {
        return (ext == ".glsl" || ext == ".shader");
    }

    template <>
    bool AssetDataManager::isValidFileExtensionForAssetType<Texture>(const std::string& ext) {
        return (ext == ".jpg" || ext == ".png");
    }

    template <>
    bool AssetDataManager::isValidFileExtensionForAssetType<Material>(const std::string& ext) {
        return (ext == ".mat");
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
                            std::vector<AssetHandleIDtype> materials = assetNode["materials"].as<std::vector<AssetHandleIDtype>>();
                            getAssetDataMap<Model>().map.insert({id, AssetData<Model>(id, assetFilepath, name, materials)});
                        }
                        else if(isValidFileExtensionForAssetType<Shader>(fileExtenstion)) {
                            getAssetDataMap<Shader>().map.insert({id, AssetData<Shader>(id, assetFilepath, name)});
                        }
                        else if(isValidFileExtensionForAssetType<Texture>(fileExtenstion)) {
                            bool srgb = assetNode["srgb"].as<bool>();
                            getAssetDataMap<Texture>().map.insert({id, AssetData<Texture>(id, assetFilepath, name, srgb)});
                        }
                        else if(isValidFileExtensionForAssetType<Material>(fileExtenstion)) {
                            getAssetDataMap<Material>().map.insert({id, AssetData<Material>(id, assetFilepath, name)});
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

    AssetHandleIDtype AssetDataManager::importAsset(const std::string& assetPath) {
        std::string ext = std::filesystem::path(assetPath).extension();
        if(isValidFileExtensionForAssetType<Model>(ext)) return importAsset<Model>(assetPath);
        else if(isValidFileExtensionForAssetType<Shader>(ext)) return importAsset<Shader>(assetPath);
        else if(isValidFileExtensionForAssetType<Texture>(ext)) return importAsset<Texture>(assetPath);
        else if(isValidFileExtensionForAssetType<Material>(ext)) return importAsset<Material>(assetPath);
        else return 0;
    }

    template <typename T>
    AssetHandleIDtype AssetDataManager::importAsset(const std::string& assetPath) {
        if(!isValidFileExtensionForAssetType<T>(std::filesystem::path(assetPath).extension())) {
            std::cout << "Could not import asset: " << assetPath << ". Invalid file extension" << std::endl;
            return 0;
        }

        // Get paths
        std::filesystem::path pathFromAssetFolder = getPathFromAssetFolder(assetPath, m_assetFolderPath);
        std::filesystem::path absolutePath = getAbsolutePath(assetPath, m_assetFolderPath);
        std::string filename = absolutePath.filename();

        if(!std::filesystem::exists(absolutePath)) {
            std::cout << "Could not import asset: " << absolutePath << ". Path does not exist" << std::endl;
            return 0;
        }

        // Make AssetData, and load in the new data from file
        auto [id, name] = getIDAndNameFromAssetDataFile(absolutePath);
        if(id == 0) {
            id = getAssetDataMap<T>().nextID();
            name = absolutePath.stem();
        }

        AssetData<T> assetData(id, absolutePath.string(), name, this);

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
        emitter << YAML::Key << "name" << YAML::Value << assetData.name;
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
        getAssetDataMap<T>().map.insert({assetData.ID, assetData});
        
        return id;
    }

    template <typename T>
    AssetData<T>* AssetDataManager::getAssetData(AssetHandleIDtype ID) {
        auto search = getAssetDataMap<T>().map.find(ID);
        if(search != getAssetDataMap<T>().map.end()) return &(search->second);
        return nullptr;
    }

    template <typename T>
    bool AssetDataManager::exists(AssetHandleIDtype ID) {
        return getAssetDataMap<T>().map.find(ID) != getAssetDataMap<T>().map.end();
    }

    EXPLICIT_TEMPLATE_INSTANTIATION(Model);
    EXPLICIT_TEMPLATE_INSTANTIATION(Shader);
    EXPLICIT_TEMPLATE_INSTANTIATION(Texture);
    EXPLICIT_TEMPLATE_INSTANTIATION(Material);

}
