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
        static void visit(const std::string& name, int* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(const std::string& name, float* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(const std::string& name, double* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(const std::string& name, bool* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(const std::string& name, char* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(const std::string& name, std::string* var, YAML::Emitter& emitter) {
            emitter << *var;
        }

        static void visit(const std::string& name, Vector2* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << YAML::EndSeq;
        }

        static void visit(const std::string& name, Vector3* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << YAML::EndSeq;
        }

        static void visit(const std::string& name, Vector4* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        static void visit(const std::string& name, Vector2i* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << YAML::EndSeq;
        }

        static void visit(const std::string& name, Vector3i* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << YAML::EndSeq;
        }

        static void visit(const std::string& name, Vector4i* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        static void visit(const std::string& name, Quaternion* var, YAML::Emitter& emitter) {
            emitter << YAML::Flow;
            emitter << YAML::BeginSeq << (*var)[0] << (*var)[1] << (*var)[2] << (*var)[3] << YAML::EndSeq;
        }

        static void visit(const std::string& name, int* selection, std::vector<std::string>& options, YAML::Emitter& emitter) {
            emitter << *selection;
        }

        static void visit(const std::string& name, ModelAsset* modelAsset, YAML::Emitter& emitter) {
            emitter << modelAsset->ID();
        }

        static void visit(const std::string& name, ShaderAsset* shaderAsset, YAML::Emitter& emitter) {
            emitter << shaderAsset->ID();
        }

        static void visit(const std::string& name, TextureAsset* textureAsset, YAML::Emitter& emitter) {
            emitter << textureAsset->ID();
        }

        static void visit(const std::string& name, MaterialAsset* materialAsset, YAML::Emitter& emitter) {
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

                for(size_t i = 0; i < component.getPublicVariableCount(); ++i) {
                    emitter << YAML::Key << component.getPublicVariable(i).getName();

                    emitter << YAML::Value;
                    component.getPublicVariable(i).visit<PublicVariableSerializeVisitor>(component, emitter);
                }
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
