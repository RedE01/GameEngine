#include "SceneWindow.h"
#include "Editor.h"
#include "../Application.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Events/EditorEvents.h"
#include "../Components/NameComponent.h"

#include <imgui.h>

namespace GameEngine {

    SceneWindow::SceneWindow(Application* application, Editor* editor) : GuiWindow(application, editor) {
        setWindowFlags(ImGuiWindowFlags_MenuBar);
    }

    void SceneWindow::renderWindow() {
        if(ImGui::BeginMenuBar()) {
            Scene* activeScene = getApplication()->getSceneManager()->getActiveScene();
            ImGui::BeginDisabled(activeScene == nullptr);

            if(ImGui::Button("Create entity")) {
                if(activeScene) {
                    activeScene->createEntity("New Entity");
                }
            }
            ImGui::EndDisabled();

            ImGui::EndMenuBar();
        }

        static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

        int nodeIndex = 0;
        getApplication()->getSceneManager()->getActiveScene()->each([&](Entity entity){
            ImGuiTreeNodeFlags nodeFlags = baseFlags;
            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

            if(entity == getEditor()->getSelectedEntity()) {
                nodeFlags |= ImGuiTreeNodeFlags_Selected;
            }

            const char* entityName = entity.getComponent<NameComponent>().getNameCStr();
            ImGui::TreeNodeEx((void*)(intptr_t)nodeIndex, nodeFlags, "%s", entityName);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                getEditor()->selectEntity(entity);
            }

            nodeIndex++;
        });
    }

}
