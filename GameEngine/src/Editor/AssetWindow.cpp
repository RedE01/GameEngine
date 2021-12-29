#include "AssetWindow.h"
#include "../Application.h"
#include "../Assets/AssetManager.h"
#include "../Rendering/Texture.h"

#include <imgui.h>

namespace GameEngine {

    template <typename T> void showAsset(AssetHandle<T>, ImVec2) { }

    void showAsset(AssetHandle<Model>, ImVec2 size) {
        ImGui::Dummy(size);
    }

    void showAsset(AssetHandle<Shader>, ImVec2 size) {
        ImGui::Dummy(size);
    }

    void showAsset(AssetHandle<Texture> texture, ImVec2 size) {
        ImGui::Image(reinterpret_cast<ImTextureID>(texture->getTextureID()), size);
    }

    void showAsset(AssetHandle<Material>, ImVec2 size) {
        ImGui::Dummy(size);
    }


    template<typename T>
    void showAssets(AssetManager* assetManager, int guiScale) {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 size(16 * guiScale, 16 * guiScale);
        float windowVisible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        int n = 0;
        assetManager->each<T>([&](AssetHandle<T> asset){
            ImGui::PushID(n);
            ImGui::BeginGroup();

            std::string name = std::string(assetManager->getName(asset));
            std::string nameStr;
            if(name.size() > 16) nameStr = name.substr(0, 16 - 3) + "...";
            else nameStr = name;
            float textWidth = ImGui::CalcTextSize(nameStr.c_str()).x;

            ImGui::Dummy({std::max(0.0f, (textWidth - size.x)) / 2, size.y});
            ImGui::SameLine();
            showAsset(asset, size);

            ImGui::Dummy({std::max(0.0f, (size.x - textWidth)) / 2, size.y});
            ImGui::SameLine();
            ImGui::Text("%s", nameStr.c_str());

            ImGui::EndGroup();


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

            ImGui::SetNextItemWidth(64);
            ImGui::SliderInt("Gui scale", &m_guiScale, 2, 8, "", ImGuiSliderFlags_NoInput);
            ImGui::EndMenuBar();
        }

        switch(m_assetType) {
            case AssetType::Model: showAssets<Model>(getApplication()->getAssetManager(), m_guiScale); break;
            case AssetType::Shader: showAssets<Shader>(getApplication()->getAssetManager(), m_guiScale); break;
            case AssetType::Texture: showAssets<Texture>(getApplication()->getAssetManager(), m_guiScale); break;
            case AssetType::Material: showAssets<Material>(getApplication()->getAssetManager(), m_guiScale); break;
        }
    }

}
