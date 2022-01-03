#include "SceneLoader.h"
#include "AssetManager.h"
#include "../Components/ScriptComponentManager.h"

#include "../Components/NameComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/MeshRendererComponent.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace GameEngine {

    template <typename T>
    struct PublicVariableLoadVisitor {
        PublicVariableLoadVisitor(const YAML::Node& node, AssetManager* assetManager) : node(node), assetManager(assetManager) {}

        void visit(int* var) { *var = node.as<int>(*var); }
        void visit(float* var) { *var = node.as<float>(*var); }
        void visit(double* var) { *var = node.as<double>(*var); }
        void visit(bool* var) { *var = node.as<bool>(*var); }
        void visit(char* var) { *var = node.as<char>(*var); }
        void visit(std::string* var) { *var = node.as<std::string>(*var); }

        void visit(Vector2* var) {
            if(node.IsSequence() && node.size() == 2) {
                *var = Vector2(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]));
            }
        }

        void visit(Vector3* var) {
            if(node.IsSequence() && node.size() == 3) {
                *var = Vector3(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]), node[2].as<float>((*var)[2]));
            }
        }

        void visit(Vector4* var) {
            if(node.IsSequence() && node.size() == 4) {
                *var = Vector4(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]), node[2].as<float>((*var)[2]), node[3].as<float>((*var)[3]));
            }
        }

        void visit(Vector2i* var) {
            if(node.IsSequence() && node.size() == 2) {
                *var = Vector2i(node[0].as<int>((*var)[0]), node[1].as<int>((*var)[1]));
            }
        }

        void visit(Vector3i* var) {
            if(node.IsSequence() && node.size() == 3) {
                *var = Vector3i(node[0].as<int>((*var)[0]), node[1].as<int>((*var)[1]), node[2].as<int>((*var)[2]));
            }
        }

        void visit(Vector4i* var) {
            if(node.IsSequence() && node.size() == 4) {
                *var = Vector4i(node[0].as<int>((*var)[0]), node[1].as<int>((*var)[1]), node[2].as<int>((*var)[2]), node[3].as<int>((*var)[3]));
            }
        }

        void visit(Quaternion* var) {
            if(node.IsSequence() && node.size() == 4) {
                *var = Quaternion(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]), node[2].as<float>((*var)[2]), node[3].as<float>((*var)[3]));
            }
        }

        void visit(int* selection, std::vector<std::string>&) { *selection = node.as<int>(*selection); }

        void visit(ModelAsset* var) { *var = assetManager->load<Model>(node.as<std::string>()); }
        void visit(ShaderAsset* var) { *var = assetManager->load<Shader>(node.as<std::string>()); }
        void visit(TextureAsset* var) { *var = assetManager->load<Texture>(node.as<std::string>(), true); }
        void visit(MaterialAsset* var) { *var = assetManager->load<Material>(node.as<std::string>()); }

        const YAML::Node& node;
        AssetManager* assetManager;
    };

    template <typename T>
    struct PublicVariableSerializeVisitor {
        PublicVariableSerializeVisitor(YAML::Emitter& emitter, AssetManager* assetManager) : emitter(emitter), assetManager(assetManager) {}

        void visit(int* var) { emitter << *var; }
        void visit(float* var) { emitter << *var; }
        void visit(double* var) { emitter << *var; }
        void visit(bool* var) { emitter << *var; }
        void visit(char* var) { emitter << *var; }
        void visit(std::string* var) { emitter << *var; }

        void visit(Vector2* var) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << YAML::EndSeq;
        }

        void visit(Vector3* var) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << YAML::EndSeq;
        }

        void visit(Vector4* var) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        void visit(Vector2i* var) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << YAML::EndSeq;
        }

        void visit(Vector3i* var) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << YAML::EndSeq;
        }

        void visit(Vector4i* var) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        void visit(Quaternion* var) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        void visit(int* selection, std::vector<std::string>&) { emitter << *selection; }

        void visit(ModelAsset* modelAsset) { emitter << assetManager->getFilepath(*modelAsset); }
        void visit(ShaderAsset* shaderAsset) { emitter << assetManager->getFilepath(*shaderAsset); }
        void visit(TextureAsset* textureAsset) { emitter << assetManager->getFilepath(*textureAsset); }
        void visit(MaterialAsset* materialAsset) { emitter << assetManager->getFilepath(*materialAsset); }

        YAML::Emitter& emitter;
        AssetManager* assetManager;
    };

    std::shared_ptr<Scene> SceneLoader::load(const std::string& filepath, AssetManager* assetManager) const {
        YAML::Node node = YAML::LoadFile(filepath);

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

        for(const auto& entityNode : node["entities"]) {
            Entity entity = scene->createEntity(entityNode["name"].as<std::string>());

            for(const auto& componentNode : entityNode["components"]) {
                Component* newComponent = nullptr;

                const std::string& componentTypeName = componentNode["type"].as<std::string>();
                if(componentTypeName == "Transform") newComponent = &entity.getComponent<TransformComponent>();
                else if(componentTypeName == "Camera") newComponent = &entity.addComponent<CameraComponent>(ProjectionType::Perspective, 10.0, Vector2(1.0, 1.0));
                else if(componentTypeName == "Light") newComponent = &entity.addComponent<LightComponent>();
                else if(componentTypeName == "Mesh Renderer") newComponent = &entity.addComponent<MeshRendererComponent>();
                else {
                    newComponent = ScriptComponentManager::createComponent(componentTypeName, entity);
                }
                assert(newComponent != nullptr);

                for(const auto& publicVariableNode : componentNode["publicVariables"]) {
                    PublicVariable* pv = newComponent->getPublicVariable(publicVariableNode.first.as<std::string>());
                    if(pv != nullptr) {
                        pv->visit<PublicVariableLoadVisitor>(*newComponent, publicVariableNode.second, assetManager);
                    }
                }
            }
        }

        return scene;
    }

    void SceneLoader::serialize(const std::string& filepath, const std::string& sceneName, Scene* scene, AssetManager* assetManager) const {
        YAML::Emitter emitter;

        emitter << YAML::BeginMap;

        emitter << YAML::Key << "name";
        emitter << YAML::Value << sceneName;

        emitter << YAML::Key << "entities";
        emitter << YAML::Value << YAML::BeginSeq;

        scene->each([&](Entity entity){
            emitter << YAML::BeginMap;

            emitter << YAML::Key << "name";
            emitter << YAML::Value << entity.getComponent<NameComponent>().getName();

            emitter << YAML::Key << "components";
            emitter << YAML::Value << YAML::BeginSeq;

            entity.eachComponent([&](Component& component){
                emitter << YAML::BeginMap;
                emitter << YAML::Key << "type";
                emitter << YAML::Value << component.getName();

                emitter << YAML::Key << "publicVariables";
                emitter << YAML::Value << YAML::BeginMap;

                component.eachPublicVariable([&](const std::string& pvName, PublicVariable& pv){
                    emitter << YAML::Key << pvName;

                    emitter << YAML::Value;
                    pv.visit<PublicVariableSerializeVisitor>(component, emitter, assetManager);
                });
                emitter << YAML::EndMap;

                emitter << YAML::EndMap;
            });
            
            emitter << YAML::EndSeq;

            emitter << YAML::EndMap;
        });

        emitter << YAML::EndSeq;

        emitter << YAML::EndMap;

        if(emitter.good()) {
            std::ofstream file(filepath, std::ios::out | std::ios::trunc);
            if(file.is_open()) {
                file << emitter.c_str();
    
                file.close();
            }
            else {
                std::cout << "Error: Could not write to " << filepath << std::endl;
            }
        }
        else {
            std::cout << "Error: Could not serialize scene " << sceneName << std::endl;
        }
    }

}
