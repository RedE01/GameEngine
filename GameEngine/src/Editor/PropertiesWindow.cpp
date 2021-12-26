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

namespace GameEngine {

    template<class T>
    void writeComponentData(Entity entity) {
        if(entity.hasComponent<T>()) {
            ImGui::Text("%s", entity.getComponent<T>().getName().c_str());
            ImGui::Separator();
        }
    };

    PropertiesWindow::PropertiesWindow(Application* application, Editor* editor) : GuiWindow(application, editor) {
    }

    void PropertiesWindow::renderWindow() {
        Entity entity = getEditor()->getSelectedEntity();
        if(entity.isValid()) {

            writeComponentData<NameComponent>(entity);
            ImGui::Separator();

            writeComponentData<TransformComponent>(entity);
            writeComponentData<CameraComponent>(entity);
            writeComponentData<LightComponent>(entity);
            writeComponentData<MeshRendererComponent>(entity);

            ScriptComponentManager::eachComponent(getEditor()->getSelectedEntity(), [](Component& component){
                std::string cName = component.getName();
                ImGui::Text("%s", cName.c_str());
                ImGui::Separator();
            });
        }
    }

}
