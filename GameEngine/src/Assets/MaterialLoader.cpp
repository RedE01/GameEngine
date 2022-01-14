#include "MaterialLoader.h"
#include "AssetManager.h"
#include <fstream>
#include <yaml-cpp/yaml.h>

#include <iostream>

namespace GameEngine {

    std::shared_ptr<Material> MaterialLoader::load(AssetData<Material>* assetData, AssetManager* assetManager) const {
        YAML::Node node = YAML::LoadFile(assetData->filepath);

        std::shared_ptr<Material> material = std::make_shared<Material>();

        auto loadTexture = [&](const std::string& textureName) {
            AssetHandleIDtype textureID = node[textureName][0].as<AssetHandleIDtype>();
            AssetHandleIDtype textureLocalID = node[textureName][1].as<AssetHandleIDtype>();
            return assetManager->load<Texture>(textureID, textureLocalID);
        };
        if(node["diffuseTexture"]) material->diffuseTexture = loadTexture("diffuseTexture");
        if(node["normalTexture"]) material->normalTexture = loadTexture("normalTexture");

        return material;
    }

    void MaterialLoader::save(Material* material, const std::string& filepath) const {
        std::ofstream file(filepath, std::ios::out | std::ios::trunc);
        if(file.is_open()) {
            YAML::Emitter emitter;
            
            emitter << YAML::BeginMap;

            emitter << YAML::Key << "diffuseTexture";
            emitter << YAML::Value << YAML::Flow << YAML::BeginSeq;
            emitter << material->diffuseTexture.ID() << material->diffuseTexture.localID();
            emitter << YAML::EndSeq;

            emitter << YAML::Key << "normalTexture";
            emitter << YAML::Value << YAML::Flow << YAML::BeginSeq;
            emitter << material->normalTexture.ID() << material->normalTexture.localID();
            emitter << YAML::EndSeq;

            emitter << YAML::EndMap;

            file << emitter.c_str();

            file.close();
        }
        else std::cout << "Could not open file " << filepath << std::endl;

    }

}
