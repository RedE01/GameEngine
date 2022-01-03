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
        PublicVariablePropertiesVisitor(const std::string& name) : name(name) {}

        void visit(int* var) {
            ImGui::DragInt(name.c_str(), var);
        }

        void visit(float* var) {
            ImGui::DragFloat(name.c_str(), var);
        }

        void visit(double* var) {
            ImGui::InputDouble(name.c_str(), var);
        }

        void visit(bool* var) {
            ImGui::Checkbox(name.c_str(), var);
        }

        void visit(char* var) {
            char buff[2] = {*var, 0};
            ImGui::PushItemWidth(16);
            if(ImGui::InputText(name.c_str(), buff, 2)) {
                *var = buff[0];
            }
            ImGui::PopItemWidth();
        }

        void visit(std::string* var) {
            ImGui::InputText(name.c_str(), var);
        }

        void visit(Vector2* var) {
            ImGui::DragFloat2(name.c_str(), &((*var)[0]));
        }

        void visit(Vector3* var) {
            ImGui::DragFloat3(name.c_str(), &((*var)[0]));
        }

        void visit(Vector4* var) {
            ImGui::DragFloat4(name.c_str(), &((*var)[0]));
        }

        void visit(Vector2i* var) {
            ImGui::DragInt2(name.c_str(), &((*var)[0]));
        }

        void visit(Vector3i* var) {
            ImGui::DragInt3(name.c_str(), &((*var)[0]));
        }

        void visit(Vector4i* var) {
            ImGui::DragInt4(name.c_str(), &((*var)[0]));
        }

        void visit(Quaternion* var) {
            Vector3 eulerAnglesPrev = glm::degrees(glm::eulerAngles(*var));
            Vector3 eulerAngles = eulerAnglesPrev;
            if(ImGui::DragFloat3(name.c_str(), &(eulerAngles[0]))) {
                Vector3 deltaEulerAngles = eulerAngles - eulerAnglesPrev;
                Quaternion rotation = Quaternion(glm::radians(deltaEulerAngles));

                *var *= rotation;
            }
        }

        void visit(int* selection, std::vector<std::string>& options) {
            std::vector<const char*> optionsCharPtr;
            for(auto& option : options) optionsCharPtr.push_back(option.c_str());
            ImGui::Combo(name.c_str(), selection, optionsCharPtr.data(), optionsCharPtr.size());
        }

        void visit(ModelAsset*) {
            ImGui::Text("name: %s", name.c_str());
        }

        void visit(ShaderAsset*) {
            ImGui::Text("name: %s", name.c_str());
        }

        void visit(TextureAsset*) {
            ImGui::Text("name: %s", name.c_str());
        }

        void visit(MaterialAsset*) {
            ImGui::Text("name: %s", name.c_str());
        }

        const std::string& name;
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
