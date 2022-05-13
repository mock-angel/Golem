/*
 * GuiManager.h
 *
 *  Created on: 06-May-2022
 *      Author: anantha
 */

#ifndef GUI_GUIMANAGER_H_
#define GUI_GUIMANAGER_H_

#include <list>
#include <memory>

#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Window/GuiWindow.h"

class GuiManager
{
	std::list<std::shared_ptr<GuiWindow>> m_guiWindows;

public:
	GuiManager();
	virtual ~GuiManager();

	void addGuiWindow(std::shared_ptr<GuiWindow> t_guiWindow);

	void handleEvent(SDL_Event &e);
	void render();
	void onGui();
};

#endif /* GUI_GUIMANAGER_H_ */
