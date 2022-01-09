#include "AssetData.h"
#include "AssetDataManager.h"
#include "../Rendering/Model.h"
#include "MaterialLoader.h"

#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GameEngine {

    Material processMaterial(aiMaterial* aiMat, const aiScene* scene, const std::filesystem::path& baseFolderPath, AssetDataManager* assetDataManager) {
        Material newMat;

        auto getTexture = [&](aiTextureType type, bool srgb) -> AssetHandleIDtype {
            aiString aiTexturePathStr("");

            if(aiMat->GetTextureCount(type) > 0) {
                if(aiMat->GetTexture(type, 0, &aiTexturePathStr) == aiReturn_SUCCESS) {
                    const char* texturePathStr = aiTexturePathStr.C_Str();
                    const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(texturePathStr);

                    if(embeddedTexture) { // If the texture is embedded
                        unsigned int dataLength = embeddedTexture->mWidth;
                        if(embeddedTexture->mHeight != 0) dataLength *= embeddedTexture->mHeight; // If the image is compressed, mHeight = 0 and mWidth is the dataLength

                        assert(false); // Embedded textures have not been implemented yet
                    }
                    else { // Texture is not embedded
                        return assetDataManager->importAsset<Texture>(baseFolderPath / std::string(texturePathStr));
                    }
                }
            }

            return 0;
        };

        newMat.diffuseTexture = TextureAsset(getTexture(aiTextureType::aiTextureType_DIFFUSE, true), {});
        newMat.normalTexture = TextureAsset(getTexture(aiTextureType::aiTextureType_NORMALS, false), {});

        return newMat;
    }


    AssetData<Model>::AssetData(AssetHandleIDtype ID, const std::string& filepath, const std::string& name, AssetDataManager* assetDataManager)
        : AssetDataBase(ID, filepath, name) {

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filepath, 0);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << importer.GetErrorString() << std::endl;
            return;
        }

        std::filesystem::path baseFolderPathAbsolute = std::filesystem::absolute(filepath).parent_path();

        for(unsigned int i = 0; i < scene->mNumMaterials; ++i) {
            aiMaterial* aiMat = scene->mMaterials[i];

            Material newMat = processMaterial(aiMat, scene, baseFolderPathAbsolute, assetDataManager);
            std::string materialName = std::string(aiMat->GetName().C_Str()) + ".mat";

            MaterialLoader ml;
            ml.save(&newMat, baseFolderPathAbsolute / materialName);
            AssetHandleIDtype materialID = assetDataManager->importAsset<Material>(baseFolderPathAbsolute / materialName);
            
            materialIDs.push_back(materialID);
        }
    }

}
