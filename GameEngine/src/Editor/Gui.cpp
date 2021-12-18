#include "Gui.h"
#include "../Window.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace GameEngine {

    Gui::Gui(Window* window) {
        m_windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar 
                | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus 
                | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        init(window);
    }

    void Gui::OnGuiRender() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        
        ImGui::NewFrame();

        //Viewport
        bool viewportOpen = true;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpaceBegin", &viewportOpen, m_windowFlags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiID dockspaceID = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0, 0.0));

        //Menu bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if(ImGui::SmallButton("save scene")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Options")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows")) {
                for(auto& windowList : m_windows) {
                    for(auto& window : windowList.second) {
                        const char* windowName = window->getWindowName();

                        if(window->isOpen()) {
                            ImGui::Text("%s", windowName);
                        }
                        else {
                            if(ImGui::SmallButton(windowName)) {
                                window->setOpen(true);
                            }
                        }
                    }
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        //Render Windows
        for(auto& windowList : m_windows) {
            for(auto& window : windowList.second) {
                if(window->isOpen()) {
                    bool windowIsOpen = window->isOpen();
                    ImGui::Begin(window->getWindowName(), &windowIsOpen, window->getWindowFlags());
                    window->setOpen(windowIsOpen);
                    window->renderWindow();
                    ImGui::End();
                }
            }
        }


        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Gui::init(Window* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        GLFWwindow* glfwWindow = window->getWindow();
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init(NULL);

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigDockingWithShift = false;
    }

}
