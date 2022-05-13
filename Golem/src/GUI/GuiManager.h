/*
 * GuiManager.h
 *
 *  Created on: 06-May-2022
 *      Author: sujak
 */

#ifndef GUI_GUIMANAGER_H_
#define GUI_GUIMANAGER_H_

#include <list>

#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include "imgui/backends/imgui_impl_opengl3.h"

#include "GuiWindow.h"

class GuiManager
{
	std::list<GuiWindow> m_guiWindows;

public:
	GuiManager();
	virtual ~GuiManager();

	void handleEvent(SDL_Event &e);
	void render();
};

#endif /* GUI_GUIMANAGER_H_ */
