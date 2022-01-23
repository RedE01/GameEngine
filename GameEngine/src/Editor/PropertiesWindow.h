#pragma once
#include "GuiWindow.h"

namespace GameEngine {

    class PropertiesWindow : public GuiWindow {
    public:
        PropertiesWindow(Application* application, Editor* editor);

        inline virtual const char* getWindowName() override { return "Properties"; }
        virtual void renderWindow() override;

    private:
        void renderEntityProperties();
        void renderAssetProperties();
        void renderSettings();

    private:
        int m_renamingBeginFrame = 0;
    };


}
