#include "AssetImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Rendering/Model.h"

namespace GameEngine {
    namespace assetImporter {

        std::unique_ptr<Mesh> processMesh(aiMesh* meshPtr) {
            std::vector<Vertex> verticies;
            std::vector<unsigned int> indicies;
            verticies.reserve(meshPtr->mNumVertices);

            for(unsigned int i = 0; i < meshPtr->mNumVertices; ++i) {
                Vertex v;
                v.position = glm::vec3(meshPtr->mVertices[i].x, meshPtr->mVertices[i].y, meshPtr->mVertices[i].z);
                if(meshPtr->HasNormals()) v.normal = glm::vec3(meshPtr->mNormals[i].x, meshPtr->mNormals[i].y, meshPtr->mNormals[i].z);
                if(meshPtr->HasTextureCoords(0)) v.textureCoords = glm::vec2(meshPtr->mTextureCoords[0][i].x, meshPtr->mTextureCoords[0][i].y);

                verticies.push_back(v);
            }

            for(unsigned int i = 0; i < meshPtr->mNumFaces; ++i) {
                aiFace face = meshPtr->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; ++j) {
                    indicies.push_back(face.mIndices[j]);
                }
            }

            return std::make_unique<Mesh>(verticies, indicies);
        }

        std::unique_ptr<Model> importModel(const char* filepath) {
            Assimp::Importer importer;

            const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cout << importer.GetErrorString() << std::endl;
                return {};
            }

            std::unique_ptr<Model> model = std::make_unique<Model>();
            for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
                model->meshes.push_back(processMesh(scene->mMeshes[i]));
            }
            
            return model;
        }
    }
}