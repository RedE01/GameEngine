#include "PropertiesWindow.h"
#include "Editor.h"
#include "../Application.h"
#include "../Scene/Scene.h"
#include "../Events/EditorEvents.h"
#include "../Components/ScriptComponentManager.h"
#include "../Assets/AssetManager.h"
#include "../Rendering/Renderer.h"

#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/MeshRendererComponent.h"
#include "../Components/NameComponent.h"

#include <unordered_set>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "../Rendering/Model.h"
#include "../Rendering/Material.h"

template<class> inline constexpr bool always_false_v = false;

namespace GameEngine {

    template <typename T>
    struct PublicVariablePropertiesVisitor {
        PublicVariablePropertiesVisitor(const std::string& name, AssetManager* assetManager) : name(name), assetManager(assetManager) {}

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

        void visit(ModelAsset* var) {
            ImGui::Text("%s: %s", name.c_str(), assetManager->getName(*var));
            if(ImGui::BeginDragDropTarget()) {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_ASSET_PAYLOAD")) {
                    using assetIDsType = std::pair<AssetHandleIDtype, AssetHandleIDtype>;
                    assetIDsType assetIDs = *static_cast<assetIDsType*>(payload->Data);
                    *var = assetManager->load<Model>(assetIDs.first, assetIDs.second);
                }
            }
        }

        void visit(ShaderAsset* var) {
            ImGui::Text("%s: %s", name.c_str(), assetManager->getName(*var));
            if(ImGui::BeginDragDropTarget()) {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER_ASSET_PAYLOAD")) {
                    using assetIDsType = std::pair<AssetHandleIDtype, AssetHandleIDtype>;
                    assetIDsType assetIDs = *static_cast<assetIDsType*>(payload->Data);
                    *var = assetManager->load<Shader>(assetIDs.first, assetIDs.second);
                }
            }
        }

        void visit(TextureAsset* var) {
            ImGui::Text("%s: %s", name.c_str(), assetManager->getName(*var));
            if(ImGui::BeginDragDropTarget()) {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_ASSET_PAYLOAD")) {
                    using assetIDsType = std::pair<AssetHandleIDtype, AssetHandleIDtype>;
                    assetIDsType assetIDs = *static_cast<assetIDsType*>(payload->Data);
                    *var = assetManager->load<Texture>(assetIDs.first, assetIDs.second);
                }
            }
        }

        void visit(MaterialAsset* var) {
            ImGui::Text("%s: %s", name.c_str(), assetManager->getName(*var));
            if(ImGui::BeginDragDropTarget()) {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL_ASSET_PAYLOAD")) {
                    using assetIDsType = std::pair<AssetHandleIDtype, AssetHandleIDtype>;
                    assetIDsType assetIDs = *static_cast<assetIDsType*>(payload->Data);
                    *var = assetManager->load<Material>(assetIDs.first, assetIDs.second);
                }
            }
        }

        const std::string name;
        AssetManager* assetManager;
    };

    template <typename T>
    void renderAddComponentSelectable(Entity& entity) {
        ImGui::BeginDisabled(entity.hasComponent<T>());
        if(ImGui::Selectable(T::GetName().c_str())) {
            entity.addComponent<T>();
        }
        ImGui::EndDisabled();

    }

    PropertiesWindow::PropertiesWindow(Application* application, Editor* editor) : GuiWindow(application, editor) {
    }

    void PropertiesWindow::renderWindow() {
        if(ImGui::BeginTabBar("##tabs")) {
            if(ImGui::BeginTabItem("Properties")) {
                renderEntityProperties();
                renderAssetProperties();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Settings")) {
                renderSettings();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    void PropertiesWindow::renderEntityProperties() {
        Entity entity = getEditor()->getSelectedEntity();
        if(entity.isValid()) {
            if(m_renamingBeginFrame == 0) {
                ImGui::Text("%s", entity.getComponent<NameComponent>().getNameCStr());
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    m_renamingBeginFrame = ImGui::GetFrameCount() + 1;
                }
            }
            else {
                if(m_renamingBeginFrame == ImGui::GetFrameCount()) ImGui::SetKeyboardFocusHere();
                ImGui::InputText("##Entity name", &entity.getComponent<NameComponent>().name);

                if(ImGui::IsItemDeactivated()) {
                    m_renamingBeginFrame = 0;
                }
            }
            ImGui::Separator();
            ImGui::Separator();

            entity.eachComponent([&](Component& component){
                std::string cName = component.getName();
                ImGui::Text("%s", cName.c_str());
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 16);
                ImGui::PushID(cName.c_str());
                if(ImGui::Button("X")) {
                    entity.removeComponent(component);
                }
                ImGui::PopID();

                component.eachPublicVariable([&](const std::string& pvName, PublicVariable& pv){
                    pv.visit<PublicVariablePropertiesVisitor>(component, pvName, getApplication()->getAssetManager());
                });

                ImGui::Separator();
            });

            if(ImGui::Button("Add Component")) {
                ImGui::OpenPopup("addComponentPopup");
            }

            if(ImGui::BeginPopup("addComponentPopup")) {
                ImGui::Text("Add Component");
                ImGui::Separator();

                renderAddComponentSelectable<CameraComponent>(entity);
                renderAddComponentSelectable<LightComponent>(entity);
                renderAddComponentSelectable<MeshRendererComponent>(entity);

                std::unordered_set<std::string> componentAlreadyExists;
                entity.eachComponent([&](Component& component) {
                    componentAlreadyExists.insert(component.getName());
                });

                ScriptComponentManager::eachComponentTypeName([&](const std::string& componentTypeName) {
                    ImGui::BeginDisabled(componentAlreadyExists.contains(componentTypeName));
                    if(ImGui::Selectable(componentTypeName.c_str())) {
                        ScriptComponentManager::createComponent(componentTypeName, entity);
                    }
                    ImGui::EndDisabled();
                });
                ImGui::EndPopup();
            }
        }
    }

    void PropertiesWindow::renderAssetProperties() { 
        AssetVariant assetVariant = getEditor()->getSelectedAsset();

        AssetManager* assetManager = getApplication()->getAssetManager();
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if(!arg) return;

            auto renderAssetName = [&](const char* assetTypeName) {
                ImGui::Text("%s: %s", assetTypeName, assetManager->getName(arg));
                ImGui::Separator();
                ImGui::Separator();
            };

            if constexpr(std::is_same_v<T, ModelAsset>) {
                ModelAsset asset = static_cast<ModelAsset>(arg);
                renderAssetName("Model");

                for(size_t i = 0; i < asset->meshes.size(); ++i) {
                    ImGui::Text("Mesh %lu", i);
                    ImGui::Text("IndexCount: %i", asset->meshes[i]->getIndexCount());

                    PublicVariablePropertiesVisitor<MaterialAsset> v("Material: ", assetManager);
                    v.visit(&(asset->meshes[i]->material));
                }
                ImGui::Separator();
            }
            else if constexpr(std::is_same_v<T, ShaderAsset>) {
                renderAssetName("Shader");
            }
            else if constexpr(std::is_same_v<T, TextureAsset>) {
                renderAssetName("Texture");
            }
            else if constexpr(std::is_same_v<T, MaterialAsset>) {
                MaterialAsset asset = static_cast<MaterialAsset>(arg);
                renderAssetName("Material");

                PublicVariablePropertiesVisitor<TextureAsset> v1("Diffuse texture: ", assetManager);
                v1.visit(static_cast<TextureAsset*>(&(asset->diffuseTexture)));

                PublicVariablePropertiesVisitor<TextureAsset> v2("Normal texture: ", assetManager);
                v2.visit(static_cast<TextureAsset*>(&(asset->normalTexture)));
            }
            else static_assert(always_false_v<T>, "Non exhaustive visitor");
        }, assetVariant);
    }

    void PropertiesWindow::renderSettings() {
        ImGui::Text("Renderer");
        ImGui::DragFloat("Editor camera exposure", &(getEditor()->getEditorCamera()->exposure), 0.01, 0.0, 100.0);

        ImGui::Separator();
    }


}
