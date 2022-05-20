#include "EditorWindow.h"
#include <iostream>

#include <Core/GUI/Gui.h>
#include <Editor/GUI/Windows/TrackerGui.h>

namespace Golem
{
    void EditorWindow::onOpen()
    {
        m_guiManager.addGuiWindow(std::make_shared<TrackerGui>());
    }

    void EditorWindow::onEvent(SDL_Event &e)
    {
        Timer timer("EditorWindow::onEvent");
        m_guiManager.handleEvent(e);
    }

    void EditorWindow::onUpdate()
    {
    }

    void EditorWindow::onGui()
    {
        // return;
        int dockspaceID;
        bool active = true;
        int win_width, win_height;
        SDL_GetWindowSize(Window::getSDLWindow(), &win_width, &win_height);

        ImGui::SetNextWindowSize(ImVec2(win_width, win_height));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        if (ImGui::Begin("Master Window", &active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus))
        {
            ImGui::TextUnformatted("DockSpace below");
        }
        if (active)
        {
            dockspaceID = ImGui::GetID("HUB_DockSpace");
            ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode /*|ImGuiDockNodeFlags_NoResize*/);
        }
        ShowGui(); // ShowExampleAppMainMenuBar();

        ImGui::End();

        ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
        m_guiManager.onGui();
    }
} // namespace Golem
