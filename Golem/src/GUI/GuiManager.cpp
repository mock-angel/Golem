/*
 * GuiManager.cpp
 *
 *  Created on: 06-May-2022
 *      Author: sujak
 */

#include "GuiManager.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

GuiManager::GuiManager()
{
	// TODO Auto-generated constructor stub
}

GuiManager::~GuiManager()
{
	// TODO Auto-generated destructor stub
}

void GuiManager::handleEvent(SDL_Event &e)
{
	if (e.type == SDL_WINDOWEVENT)
	{
		// handleWindowEvent(e);

		// FIXME: This should prolly not be here.
		if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			ImGuiIO &io = ImGui::GetIO();
			io.DisplaySize = ImVec2(e.window.data1, e.window.data1);
		}
	}
	ImGui_ImplSDL2_ProcessEvent(&e);
}

