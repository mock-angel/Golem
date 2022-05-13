/*
 * Gui.h
 *
 *  Created on: 06-May-2022
 *      Author: anantha
 */

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

#include <SDL2/SDL.h>
#include <imgui.h>

class Gui
{
public:
	Gui();
	virtual ~Gui();

	static SDL_Window *sdl_window;
	static ImGuiContext *imgui_context;

	static void Init(SDL_Window *t_sdlwindow);
	static void InitForGL(SDL_Window *t_sdlwindow, SDL_GLContext context);
	static void InitForVulkan(SDL_Window *t_sdlwindow);
	static void Cleanup();

	static void NewFrame();
	// static void RenderGUI();
};

void ShowGui();

#endif /* GUI_GUI_H_ */
