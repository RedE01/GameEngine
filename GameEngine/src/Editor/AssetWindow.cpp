#include "AssetWindow.h"
#include "Editor.h"
#include "../Application.h"
#include "../Assets/AssetManager.h"
#include "../Rendering/Texture.h"
#include <filesystem>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace GameEngine {

    template <typename T> void showAsset(AssetData<T>&, AssetHandle<T>, ImVec2) { }

    void showAsset(AssetData<Model>&, AssetHandle<Model>, ImVec2 size) {
        ImGui::Dummy(size);
    }

    void showAsset(AssetData<Shader>&, AssetHandle<Shader>, ImVec2 size) {
        ImGui::Dummy(size);
    }

    void showAsset(AssetData<Texture>&, AssetHandle<Texture> assetHandle, ImVec2 size) {
        if(assetHandle) ImGui::Image(reinterpret_cast<ImTextureID>(assetHandle->getTextureID()), size);
        else ImGui::Dummy(size);
    }

    void showAsset(AssetData<Material>&, AssetHandle<Material>, ImVec2 size) {
        ImGui::Dummy(size);
    }

    template <typename T> const char* getAssetDragDropPayloadString();
    template <> const char* getAssetDragDropPayloadString<Model>() { return "MODEL_ASSET_PAYLOAD"; }
    template <> const char* getAssetDragDropPayloadString<Shader>() { return "SHADER_ASSET_PAYLOAD"; }
    template <> const char* getAssetDragDropPayloadString<Texture>() { return "TEXTURE_ASSET_PAYLOAD"; }
    template <> const char* getAssetDragDropPayloadString<Material>() { return "MATERIAL_ASSET_PAYLOAD"; }

    template<typename T>
    void showAssets(AssetManager* assetManager, Editor* editor, int guiScale) {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 size(16 * guiScale, 16 * guiScale);
        float windowVisible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        int n = 0;
        assetManager->each<T>([&](AssetData<T>& assetData) {
            ImGui::PushID(n);
            ImGui::BeginGroup();

            std::string nameStr;
            if(assetData.name.size() > 16) nameStr = assetData.name.substr(0, 16 - 3) + "...";
            else nameStr = assetData.name;
            float textWidth = ImGui::CalcTextSize(nameStr.c_str()).x;

            ImGui::Dummy({std::max(0.0f, (textWidth - size.x)) / 2, size.y});
            ImGui::SameLine();
            showAsset(assetData, assetManager->getHandle<T>(assetData.ID, assetData.localID), size);
            ImGuiDragDropFlags dragDropFlags = ImGuiDragDropFlags_SourceAllowNullID;
            if(ImGui::BeginDragDropSource(dragDropFlags)) {
                ImGui::Text("%s", nameStr.c_str());

                
                using assetIDsType = std::pair<AssetHandleIDtype, AssetHandleIDtype>;
                assetIDsType assetIDs = {assetData.ID, assetData.localID};
                ImGui::SetDragDropPayload(getAssetDragDropPayloadString<T>(), &(assetIDs), sizeof(assetIDs));
                ImGui::EndDragDropSource();
            }

            ImGui::Dummy({std::max(0.0f, (size.x - textWidth)) / 2, size.y});
            ImGui::SameLine();
            ImGui::Text("%s", nameStr.c_str());

            ImGui::EndGroup();

            if(ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
                AssetHandle<T> assetHandle = assetManager->getHandle<T>(assetData.ID);
                editor->selectAsset(assetHandle);
            }


            float lastGroup = ImGui::GetItemRectMax().x;
            float nextGroup = lastGroup + style.ItemSpacing.x + size.x;
            if (nextGroup < windowVisible) {
                ImGui::SameLine();
            }

            ImGui::PopID();

            n++;
        });
    }

    AssetWindow::AssetWindow(Application* application, Editor* editor) : GuiWindow(application, editor) {
        setWindowFlags(getWindowFlags() | ImGuiWindowFlags_MenuBar);
    }

    void AssetWindow::renderWindow() {
        if (ImGui::BeginMenuBar()) {
            auto getAssetTypeStr = [](AssetType assetType) {
                switch(assetType) {
                    case AssetType::Model: return "Models";
                    case AssetType::Shader: return "Shaders";
                    case AssetType::Texture: return "Textures";
                    case AssetType::Material: return "Materials";
                };
                return "";
            };

            if(ImGui::BeginMenu(getAssetTypeStr(m_assetType))) {
                auto createAssetTypeMenu = [&](AssetType assetType) {
                    bool assetSelected = m_assetType == assetType;
                    ImGui::MenuItem(getAssetTypeStr(assetType), "", &assetSelected);
                    if(assetSelected) m_assetType = assetType;
                };

                createAssetTypeMenu(AssetType::Model);
                createAssetTypeMenu(AssetType::Shader);
                createAssetTypeMenu(AssetType::Texture);
                createAssetTypeMenu(AssetType::Material);

                ImGui::EndMenu();
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if(ImGui::Button("Import asset")) {
                m_assetToBeImportedPath = "";
                m_generatedMaterialsPath = "";
                ImGui::OpenPopup("Import asset");
            }

            if(ImGui::BeginPopupModal("Import asset")) {
                ImGui::InputText("Filepath", &m_assetToBeImportedPath);

                auto renderImportButtonFn = [](bool disabled, std::function<void(void)> importFn) {
                    ImGui::BeginDisabled(disabled);

                    if(ImGui::Button("Import", ImVec2(120, 0))) {
                        importFn();

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndDisabled();
                };

                std::string extension = std::filesystem::path(m_assetToBeImportedPath).extension().string();
                if(isValidFileExtensionForAssetType<Model>(extension)) {
                    ImGui::Checkbox("Import Materials?", &m_importMaterials);
                    if(m_importMaterials) {
                        ImGui::InputText("Material directory", &m_generatedMaterialsPath);
                    }

                    renderImportButtonFn(false, [&](){
                        getApplication()->getAssetManager()->import<Model>(m_assetToBeImportedPath, ImportSettings<Model>(m_importMaterials, m_generatedMaterialsPath));
                    });
                }
                else if(isValidFileExtensionForAssetType<Shader>(extension)) {
                    renderImportButtonFn(false, [&](){ getApplication()->getAssetManager()->import<Shader>(m_assetToBeImportedPath, {}); });
                }
                else if(isValidFileExtensionForAssetType<Texture>(extension)) {
                    ImGui::Checkbox("srgb?", &m_srgb);
                    renderImportButtonFn(false, [&](){ getApplication()->getAssetManager()->import<Texture>(m_assetToBeImportedPath, ImportSettings<Texture>(m_srgb)); });
                }
                else if(isValidFileExtensionForAssetType<Material>(extension)) {
                    renderImportButtonFn(false, [&](){ getApplication()->getAssetManager()->import<Material>(m_assetToBeImportedPath, {}); });
                }
                else renderImportButtonFn(true, [](){});

                ImGui::SameLine();
                if(ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::SetNextItemWidth(64);
            ImGui::SliderInt("Gui scale", &m_guiScale, 2, 8, "", ImGuiSliderFlags_NoInput);
            ImGui::EndMenuBar();
        }

        switch(m_assetType) {
            case AssetType::Model: showAssets<Model>(getApplication()->getAssetManager(), getEditor(), m_guiScale); break;
            case AssetType::Shader: showAssets<Shader>(getApplication()->getAssetManager(), getEditor(), m_guiScale); break;
            case AssetType::Texture: showAssets<Texture>(getApplication()->getAssetManager(), getEditor(), m_guiScale); break;
            case AssetType::Material: showAssets<Material>(getApplication()->getAssetManager(), getEditor(), m_guiScale); break;
        }
    }

}
