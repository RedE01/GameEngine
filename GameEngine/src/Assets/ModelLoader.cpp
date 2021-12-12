#include "ModelLoader.h"
#include "AssetManager.h"
#include "../Rendering/Material.h"
#include "../Rendering/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GameEngine {

	void processMaterial(AssetManager* assetManager, aiMaterial* material, const aiScene* scene) {
		if(material->GetName().length == 0) return;
		MaterialAsset materialAsset = assetManager->load<Material>(std::string(material->GetName().C_Str()));
		if(!materialAsset) return;

        auto getTexture = [material, scene, assetManager](aiTextureType type, bool srgb) {
            aiString aiTexturePathStr("");

            if(material->GetTextureCount(type) > 0) {
                if(material->GetTexture(type, 0, &aiTexturePathStr) == aiReturn_SUCCESS) {
                    const char* texturePathStr = aiTexturePathStr.C_Str();
                    const aiTexture* texture = scene->GetEmbeddedTexture(texturePathStr);

                    if(texture) { // If the texture is embedded
                        unsigned int dataLength = texture->mWidth;
                        if(texture->mHeight != 0) dataLength *= texture->mHeight; // If the image is compressed, mHeight = 0 and mWidth is the dataLength

                        return assetManager->load<Texture>(std::string(texture->mFilename.C_Str()), (unsigned char*)texture->pcData, dataLength, srgb);
                    }
                    else { // Texture is not embedded
                        return assetManager->load<Texture>(std::string(texturePathStr), srgb);
                    }
                }
            }

            return TextureAsset();
        };

        materialAsset->diffuseTexture = getTexture(aiTextureType::aiTextureType_DIFFUSE, true);
        materialAsset->normalTexture = getTexture(aiTextureType::aiTextureType_NORMALS, false);

		materialAsset->shader = assetManager->load<Shader>("Game/assets/shaders/shader.glsl");
    }

	std::unique_ptr<Mesh> processMesh(aiMesh* meshPtr, const aiScene* scene, AssetManager* assetManager) {
		std::vector<Vertex> verticies;
		std::vector<unsigned int> indicies;
		verticies.reserve(meshPtr->mNumVertices);

		for(unsigned int i = 0; i < meshPtr->mNumVertices; ++i) {
			Vertex v;
			v.position = Vector3(meshPtr->mVertices[i].x, meshPtr->mVertices[i].y, meshPtr->mVertices[i].z);
			if(meshPtr->HasNormals()) v.normal = Vector3(meshPtr->mNormals[i].x, meshPtr->mNormals[i].y, meshPtr->mNormals[i].z);
			if(meshPtr->HasTextureCoords(0)) v.textureCoords = Vector2(meshPtr->mTextureCoords[0][i].x, meshPtr->mTextureCoords[0][i].y);
            if(meshPtr->HasTangentsAndBitangents()) v.tangent = Vector3(meshPtr->mTangents[i].x, meshPtr->mTangents[i].y, meshPtr->mTangents[i].z);

			verticies.push_back(v);
		}

		for(unsigned int i = 0; i < meshPtr->mNumFaces; ++i) {
			aiFace face = meshPtr->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; ++j) {
				indicies.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[meshPtr->mMaterialIndex];
		MaterialAsset materialAsset = assetManager->getHandle<Material>(std::string(material->GetName().C_Str()));

		return std::make_unique<Mesh>(verticies, indicies, materialAsset);
	}

	std::shared_ptr<Model> ModelLoader::load(const std::string& filepath, AssetManager* assetManager) const {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << importer.GetErrorString() << std::endl;
			return {};
		}

        for(unsigned int i = 0; i < scene->mNumMaterials; ++i) {
            processMaterial(assetManager, scene->mMaterials[i], scene);
        }

		std::shared_ptr<Model> model = std::make_shared<Model>();
		for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			model->meshes.push_back(processMesh(scene->mMeshes[i], scene, assetManager));
		}
		
		return model;

	}

}
