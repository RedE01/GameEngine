#pragma once
#include "GuiWindow.h"

namespace GameEngine {

    class SceneWindow : public GuiWindow {
    public:
        SceneWindow(Application* application, Editor* editor);

        inline virtual const char* getWindowName() override { return "Scene"; }
        virtual void renderWindow() override;
    };


}
