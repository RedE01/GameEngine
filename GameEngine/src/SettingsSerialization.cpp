#include "SettingsSerialization.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Assets/AssetManager.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace GameEngine {

    namespace SettingsSerialization {

        void saveSettingsToFile(const char* filepath, Application* application) {
            YAML::Emitter emitter;
            emitter << YAML::BeginMap;

            emitter << YAML::Key << "Settings";
            emitter << YAML::Value << YAML::BeginMap;

            emitter << YAML::Key << "defaultShader";
            ShaderAsset defaultShader = application->getRenderer()->getDefaultShader();
            emitter << YAML::Value << YAML::Flow << YAML::BeginSeq << defaultShader.ID() << defaultShader.localID() << YAML::EndSeq;

            emitter << YAML::EndMap;
            emitter << YAML::EndMap;

            std::ofstream filestream;
            filestream.open(filepath, std::ios::out | std::ios::trunc);

            if(filestream.is_open()) {
                filestream << emitter.c_str();

                filestream.close();
            }
        }

        void updateSettingsFromFile(const char* filepath, Application* application) {

            YAML::Node node; 
            try {
                node = YAML::LoadFile(filepath);
            }
            catch(...) {}

            if(node.IsMap()) {
                YAML::Node settingsNode = node["Settings"];

                if(settingsNode["defaultShader"]) {
                    YAML::Node defaultShaderNode = settingsNode["defaultShader"];
                    ShaderAsset defaultShader = application->getAssetManager()->load<Shader>(defaultShaderNode[0].as<AssetHandleIDtype>(), defaultShaderNode[1].as<AssetHandleIDtype>());
                    application->getRenderer()->setDefaultShader(defaultShader);
                }

            }
        }

    }

}
