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
    struct PublicVariableSerializeVisitor {
        static void visit(int* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(float* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(double* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(bool* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(char* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(std::string* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

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

        static void visit(int* selection, std::vector<std::string>&, YAML::Emitter& emitter) {
            emitter << *selection;
        }

        static void visit(ModelAsset* modelAsset, YAML::Emitter& emitter) {
            emitter << modelAsset->ID();
        }

        static void visit(ShaderAsset* shaderAsset, YAML::Emitter& emitter) {
            emitter << shaderAsset->ID();
        }

        static void visit(TextureAsset* textureAsset, YAML::Emitter& emitter) {
            emitter << textureAsset->ID();
        }

        static void visit(MaterialAsset* materialAsset, YAML::Emitter& emitter) {
            emitter << materialAsset->ID();
        }
    };

    std::shared_ptr<Scene> SceneLoader::load(const std::string& filepath) const {
        return {};
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
