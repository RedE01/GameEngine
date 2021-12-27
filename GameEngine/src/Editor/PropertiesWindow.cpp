#include "PropertiesWindow.h"
#include "Editor.h"
#include "../Application.h"
#include "../Scene.h"
#include "../Events/EditorEvents.h"
#include "../Components/ScriptComponentManager.h"

#include "../Components/NameComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/MeshRendererComponent.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "../Rendering/Model.h"

namespace GameEngine {

    template <typename T>
    struct PublicVariableVisitor {
        static void visit(const std::string& name, int* var) {
            ImGui::DragInt(name.c_str(), var);
        }

        static void visit(const std::string& name, float* var) {
            ImGui::DragFloat(name.c_str(), var);
        }

        static void visit(const std::string& name, double* var) {
            ImGui::InputDouble(name.c_str(), var);
        }

        static void visit(const std::string& name, bool* var) {
            ImGui::Checkbox(name.c_str(), var);
        }

        static void visit(const std::string& name, char* var) {
            char buff[2] = {*var, 0};
            ImGui::PushItemWidth(16);
            if(ImGui::InputText(name.c_str(), buff, 2)) {
                *var = buff[0];
            }
            ImGui::PopItemWidth();
        }

        static void visit(const std::string& name, std::string* var) {
            ImGui::InputText(name.c_str(), var);
        }

        static void visit(const std::string& name, Vector2* var) {
            ImGui::DragFloat2(name.c_str(), &((*var)[0]));
        }

        static void visit(const std::string& name, Vector3* var) {
            ImGui::DragFloat3(name.c_str(), &((*var)[0]));
        }

        static void visit(const std::string& name, Vector4* var) {
            ImGui::DragFloat4(name.c_str(), &((*var)[0]));
        }

        static void visit(const std::string& name, Vector2i* var) {
            ImGui::DragInt2(name.c_str(), &((*var)[0]));
        }

        static void visit(const std::string& name, Vector3i* var) {
            ImGui::DragInt3(name.c_str(), &((*var)[0]));
        }

        static void visit(const std::string& name, Vector4i* var) {
            ImGui::DragInt4(name.c_str(), &((*var)[0]));
        }

        static void visit(const std::string& name, Quaternion* var) {
            Vector3 eulerAnglesPrev = glm::degrees(glm::eulerAngles(*var));
            Vector3 eulerAngles = eulerAnglesPrev;
            if(ImGui::DragFloat3(name.c_str(), &(eulerAngles[0]))) {
                Vector3 deltaEulerAngles = eulerAngles - eulerAnglesPrev;
                Quaternion rotation = Quaternion(glm::radians(deltaEulerAngles));

                *var *= rotation;
            }
        }

        static void visit(const std::string& name, int* selection, std::vector<std::string>& options) {
            std::vector<const char*> optionsCharPtr;
            for(auto& option : options) optionsCharPtr.push_back(option.c_str());
            ImGui::Combo(name.c_str(), selection, optionsCharPtr.data(), optionsCharPtr.size());
        }

        static void visit(const std::string& name, ModelAsset* modelAsset) {
            ImGui::Text("name: %s", name.c_str());
        }

        static void visit(const std::string& name, ShaderAsset* shaderAsset) {
            ImGui::Text("name: %s", name.c_str());
        }

        static void visit(const std::string& name, TextureAsset* textureAsset) {
            ImGui::Text("name: %s", name.c_str());
        }

        static void visit(const std::string& name, MaterialAsset* materialAsset) {
            ImGui::Text("name: %s", name.c_str());
        }
    };


    void showComponentData(Component& component) {
        std::string cName = component.getName();
        ImGui::Text("%s", cName.c_str());

        for(size_t i = 0; i < component.getPublicVariableCount(); ++i) {
            component.getPublicVariable(i).visit<PublicVariableVisitor>(component);
        }

        ImGui::Separator();
    }

    template<class T>
    void showComponentData(Entity entity) {
        if(entity.hasComponent<T>()) {
            showComponentData(entity.getComponent<T>());
        }
    };

    PropertiesWindow::PropertiesWindow(Application* application, Editor* editor) : GuiWindow(application, editor) {
    }

    void PropertiesWindow::renderWindow() {
        Entity entity = getEditor()->getSelectedEntity();
        if(entity.isValid()) {

            showComponentData<NameComponent>(entity);
            ImGui::Separator();

            showComponentData<TransformComponent>(entity);
            showComponentData<CameraComponent>(entity);
            showComponentData<LightComponent>(entity);
            showComponentData<MeshRendererComponent>(entity);

            ScriptComponentManager::eachComponent(getEditor()->getSelectedEntity(), [](Component& component){
                showComponentData(component);
            });
        }
    }

}
