#pragma once
#include "GuiWindow.h"

namespace GameEngine {

    class AssetWindow : public GuiWindow {
    public:
        AssetWindow(Application* application, Editor* editor);

        inline virtual const char* getWindowName() override { return "Assets"; }
        virtual void renderWindow() override;

    private:
        enum class AssetType {
            Model, Shader, Texture, Material
        };

        int m_guiScale = 5;
        AssetType m_assetType = AssetType::Model;

        std::string m_assetToBeImportedPath;
    };


}
