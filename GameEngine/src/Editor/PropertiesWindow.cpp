#include "PropertiesWindow.h"
#include "Editor.h"
#include "../Application.h"
#include "../Scene.h"
#include "../Events/EditorEvents.h"
#include "../Components/ScriptComponentManager.h"

#include "../Components/NameComponent.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "../Rendering/Model.h"

namespace GameEngine {

    template <typename T>
    struct PublicVariablePropertiesVisitor {
        static void visit(int* var, const std::string& name) {
            ImGui::DragInt(name.c_str(), var);
        }

        static void visit(float* var, const std::string& name) {
            ImGui::DragFloat(name.c_str(), var);
        }

        static void visit(double* var, const std::string& name) {
            ImGui::InputDouble(name.c_str(), var);
        }

        static void visit(bool* var, const std::string& name) {
            ImGui::Checkbox(name.c_str(), var);
        }

        static void visit(char* var, const std::string& name) {
            char buff[2] = {*var, 0};
            ImGui::PushItemWidth(16);
            if(ImGui::InputText(name.c_str(), buff, 2)) {
                *var = buff[0];
            }
            ImGui::PopItemWidth();
        }

        static void visit(std::string* var, const std::string& name) {
            ImGui::InputText(name.c_str(), var);
        }

        static void visit(Vector2* var, const std::string& name) {
            ImGui::DragFloat2(name.c_str(), &((*var)[0]));
        }

        static void visit(Vector3* var, const std::string& name) {
            ImGui::DragFloat3(name.c_str(), &((*var)[0]));
        }

        static void visit(Vector4* var, const std::string& name) {
            ImGui::DragFloat4(name.c_str(), &((*var)[0]));
        }

        static void visit(Vector2i* var, const std::string& name) {
            ImGui::DragInt2(name.c_str(), &((*var)[0]));
        }

        static void visit(Vector3i* var, const std::string& name) {
            ImGui::DragInt3(name.c_str(), &((*var)[0]));
        }

        static void visit(Vector4i* var, const std::string& name) {
            ImGui::DragInt4(name.c_str(), &((*var)[0]));
        }

        static void visit(Quaternion* var, const std::string& name) {
            Vector3 eulerAnglesPrev = glm::degrees(glm::eulerAngles(*var));
            Vector3 eulerAngles = eulerAnglesPrev;
            if(ImGui::DragFloat3(name.c_str(), &(eulerAngles[0]))) {
                Vector3 deltaEulerAngles = eulerAngles - eulerAnglesPrev;
                Quaternion rotation = Quaternion(glm::radians(deltaEulerAngles));

                *var *= rotation;
            }
        }

        static void visit(int* selection, std::vector<std::string>& options, const std::string& name) {
            std::vector<const char*> optionsCharPtr;
            for(auto& option : options) optionsCharPtr.push_back(option.c_str());
            ImGui::Combo(name.c_str(), selection, optionsCharPtr.data(), optionsCharPtr.size());
        }

        static void visit(ModelAsset*, const std::string& name) {
            ImGui::Text("name: %s", name.c_str());
        }

        static void visit(ShaderAsset*, const std::string& name) {
            ImGui::Text("name: %s", name.c_str());
        }

        static void visit(TextureAsset*, const std::string& name) {
            ImGui::Text("name: %s", name.c_str());
        }

        static void visit(MaterialAsset*, const std::string& name) {
            ImGui::Text("name: %s", name.c_str());
        }
    };

    PropertiesWindow::PropertiesWindow(Application* application, Editor* editor) : GuiWindow(application, editor) {
    }

    void PropertiesWindow::renderWindow() {
        Entity entity = getEditor()->getSelectedEntity();
        if(entity.isValid()) {
            ImGui::Text("%s", entity.getComponent<NameComponent>().getNameCStr());
            ImGui::Separator();

            entity.eachComponent([](Component& component){
                std::string cName = component.getName();
                ImGui::Text("%s", cName.c_str());

                component.eachPublicVariable([&](const std::string& pvName, PublicVariable& pv){
                    pv.visit<PublicVariablePropertiesVisitor>(component, pvName);
                });

                ImGui::Separator();
            });
        }
    }

}
