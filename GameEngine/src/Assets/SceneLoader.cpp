#include "SceneLoader.h"
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
        static void visit(int* var, const YAML::Node& node) { *var = node.as<int>(*var); }
        static void visit(float* var, const YAML::Node& node) { *var = node.as<float>(*var); }
        static void visit(double* var, const YAML::Node& node) { *var = node.as<double>(*var); }
        static void visit(bool* var, const YAML::Node& node) { *var = node.as<bool>(*var); }
        static void visit(char* var, const YAML::Node& node) { *var = node.as<char>(*var); }
        static void visit(std::string* var, const YAML::Node& node) { *var = node.as<std::string>(*var); }

        static void visit(Vector2* var, const YAML::Node& node) {
            if(node.IsSequence() && node.size() == 2) {
                *var = Vector2(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]));
            }
        }

        static void visit(Vector3* var, const YAML::Node& node) {
            if(node.IsSequence() && node.size() == 3) {
                *var = Vector3(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]), node[2].as<float>((*var)[2]));
            }
        }

        static void visit(Vector4* var, const YAML::Node& node) {
            if(node.IsSequence() && node.size() == 4) {
                *var = Vector4(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]), node[2].as<float>((*var)[2]), node[3].as<float>((*var)[3]));
            }
        }

        static void visit(Vector2i* var, const YAML::Node& node) {
            if(node.IsSequence() && node.size() == 2) {
                *var = Vector2i(node[0].as<int>((*var)[0]), node[1].as<int>((*var)[1]));
            }
        }

        static void visit(Vector3i* var, const YAML::Node& node) {
            if(node.IsSequence() && node.size() == 3) {
                *var = Vector3i(node[0].as<int>((*var)[0]), node[1].as<int>((*var)[1]), node[2].as<int>((*var)[2]));
            }
        }

        static void visit(Vector4i* var, const YAML::Node& node) {
            if(node.IsSequence() && node.size() == 4) {
                *var = Vector4i(node[0].as<int>((*var)[0]), node[1].as<int>((*var)[1]), node[2].as<int>((*var)[2]), node[3].as<int>((*var)[3]));
            }
        }

        static void visit(Quaternion* var, const YAML::Node& node) {
            if(node.IsSequence() && node.size() == 4) {
                *var = Quaternion(node[0].as<float>((*var)[0]), node[1].as<float>((*var)[1]), node[2].as<float>((*var)[2]), node[3].as<float>((*var)[3]));
            }
        }

        static void visit(int* selection, std::vector<std::string>&, const YAML::Node& node) { *selection = node.as<int>(*selection); }

        static void visit(ModelAsset*, const YAML::Node&) { }
        static void visit(ShaderAsset*, const YAML::Node&) { }
        static void visit(TextureAsset*, const YAML::Node&) { }
        static void visit(MaterialAsset*, const YAML::Node&) { }
    };

    template <typename T>
    struct PublicVariableSerializeVisitor {
        static void visit(int* var, YAML::Emitter& emitter) { emitter << *var; }
        static void visit(float* var, YAML::Emitter& emitter) { emitter << *var; }
        static void visit(double* var, YAML::Emitter& emitter) { emitter << *var; }
        static void visit(bool* var, YAML::Emitter& emitter) { emitter << *var; }
        static void visit(char* var, YAML::Emitter& emitter) { emitter << *var; }
        static void visit(std::string* var, YAML::Emitter& emitter) { emitter << *var; }

        static void visit(Vector2* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << YAML::EndSeq;
        }

        static void visit(Vector3* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << YAML::EndSeq;
        }

        static void visit(Vector4* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        static void visit(Vector2i* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << YAML::EndSeq;
        }

        static void visit(Vector3i* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << YAML::EndSeq;
        }

        static void visit(Vector4i* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        static void visit(Quaternion* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        static void visit(int* selection, std::vector<std::string>&, YAML::Emitter& emitter) { emitter << *selection; }
        static void visit(ModelAsset* modelAsset, YAML::Emitter& emitter) { emitter << modelAsset->ID(); }
        static void visit(ShaderAsset* shaderAsset, YAML::Emitter& emitter) { emitter << shaderAsset->ID(); }
        static void visit(TextureAsset* textureAsset, YAML::Emitter& emitter) { emitter << textureAsset->ID(); }
        static void visit(MaterialAsset* materialAsset, YAML::Emitter& emitter) { emitter << materialAsset->ID(); }
    };

    std::shared_ptr<Scene> SceneLoader::load(const std::string& filepath) const {
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
                        pv->visit<PublicVariableLoadVisitor>(*newComponent, publicVariableNode.second);
                    }
                }
            }
        }

        return scene;
    }

    void SceneLoader::serialize(const std::string& filepath, const std::string& sceneName, Scene* scene) const {
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
                    pv.visit<PublicVariableSerializeVisitor>(component, emitter);
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
