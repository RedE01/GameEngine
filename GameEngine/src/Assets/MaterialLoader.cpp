#include "MaterialLoader.h"
#include "AssetManager.h"
#include <fstream>
#include <yaml-cpp/yaml.h>

#include <iostream>

namespace GameEngine {

    std::shared_ptr<Material> MaterialLoader::load(AssetData<Material>* assetData, AssetManager* assetManager) const {
        YAML::Node node = YAML::LoadFile(assetData->filepath);

        std::shared_ptr<Material> material = std::make_shared<Material>();
        if(node["diffuseTexture"]) material->diffuseTexture = assetManager->load<Texture>(node["diffuseTexture"].as<AssetHandleIDtype>());
        if(node["normalTexture"]) material->normalTexture = assetManager->load<Texture>(node["normalTexture"].as<AssetHandleIDtype>());

        return material;
    }

    void MaterialLoader::save(Material* material, const std::string& filepath) const {
        std::ofstream file(filepath, std::ios::out | std::ios::trunc);
        if(file.is_open()) {
            YAML::Emitter emitter;
            
            emitter << YAML::BeginMap;

            emitter << YAML::Key << "diffuseTexture";
            emitter << YAML::Value << material->diffuseTexture.ID();

            emitter << YAML::Key << "normalTexture";
            emitter << YAML::Value << material->normalTexture.ID();

            emitter << YAML::EndMap;

            file << emitter.c_str();

            file.close();
        }
        else std::cout << "Could not open file " << filepath << std::endl;

    }

}
