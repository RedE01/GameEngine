#include "ModelLoader.h"
#include "AssetManager.h"

#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GameEngine {

	std::unique_ptr<Mesh> processMesh(aiMesh* meshPtr, MaterialAsset materialAsset) {
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

        return std::make_unique<Mesh>(verticies, indicies, materialAsset);
	}

	std::shared_ptr<Model> ModelLoader::load(AssetData<Model>* assetData, AssetManager* assetManager) const {
        const std::string& filepath = assetData->filepath;
        const std::vector<AssetHandleIDtype>& materialIDs = assetData->materialIDs;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << importer.GetErrorString() << std::endl;
			return {};
		}

        for(unsigned int i = 0; i < scene->mNumTextures; ++i) {
            aiTexture* embeddedTexture = scene->mTextures[i];

            unsigned int dataLength = embeddedTexture->mWidth;
            if(embeddedTexture->mHeight != 0) dataLength *= embeddedTexture->mHeight; // If the image is compressed, mHeight = 0 and mWidth is the dataLength

            assetManager->load<Texture>(assetData->ID, i + 1, (unsigned char*)embeddedTexture->pcData, dataLength);
        }

		std::shared_ptr<Model> model = std::make_shared<Model>();
		for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            MaterialAsset materialAsset;
            if(mesh->mMaterialIndex < materialIDs.size()) {
                materialAsset = assetManager->load<Material>(materialIDs[mesh->mMaterialIndex], 0);
            }
			model->meshes.push_back(processMesh(mesh, materialAsset));
		}
		
		return model;

	}
}
