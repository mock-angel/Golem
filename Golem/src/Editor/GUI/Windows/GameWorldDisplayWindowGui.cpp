/*
 * GameWorldDisplayWindowGui.cpp
 *
 *  Created on: 10-May-2022
 *      Author: anantha
 */

#include "GameWorldDisplayWindowGui.h"

#include <imgui.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_opengl3.h>

GameWorldDisplayWindowGui::GameWorldDisplayWindowGui()
{
	// TODO Auto-generated constructor stub
}

GameWorldDisplayWindowGui::~GameWorldDisplayWindowGui()
{
	// TODO Auto-generated destructor stub
}

void GameWorldDisplayWindowGui::OnGui()
{
	;
}
void GameWorldDisplayWindowGui::update()
{
	;
}
void GameWorldDisplayWindowGui::render()
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	// ImGui::Image(m_frameBuffer.get(), ImVec2(m_frameBuffer.getWidth(), m_frameBuffer.getHeight()));
	// ImGui::GetWindowDrawList()->AddImage(
	// (void *)window.getRenderTexture(), ImVec2(ImGui::GetCursorScreenPos()),
	// ImVec2(ImGui::GetCursorScreenPos().x + window.getWidth() / 2, ImGui::GetCursorScreenPos().y + window.getHeight() / 2), ImVec2(0, 1), ImVec2(1, 0));
}