#include "ViewportWindow.h"
#include "../Rendering/Texture.h"

#include <imgui.h>

namespace GameEngine {

    ViewportWindow::ViewportWindow() : GuiWindow() {
    }

    ViewportWindow::~ViewportWindow() {

    }

    void ViewportWindow::setTexture(std::shared_ptr<Texture> texture) {
        m_viewportTexture = texture;
    }

    void ViewportWindow::renderWindow() {
        if(m_viewportTexture.get() != nullptr) {
            ImVec2 viewportSize = ImVec2(m_viewportTexture->getWidth(), m_viewportTexture->getHeight());
            ImGui::Image(reinterpret_cast<void*>(m_viewportTexture->getTextureID()), viewportSize, ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
        }
    }

}
