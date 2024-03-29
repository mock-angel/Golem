//============================================================================
// Name        : Golem.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Game Engine in  C++
//============================================================================

#include <iostream>
//#include <SDL2/SDL.h>
#include <memory>
#include <SOIL/image_dxt.h>
#include "Window/Window.h"
#include <Editor/EditorWindow.h>
#include <Core/Profiler/Profiler.h>

using namespace std;

// Frees media and shuts down SDL
void close();

std::shared_ptr<Golem::Window> window = NULL;
bool init()
{
	// Initialization flag
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << SDL_GetError();
		return false;
	}
	// SDL attributes, must be set before context creation.
	// FIXME: This shouldn't be here, should be in application etc?
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// window = Golem::EditorWindow();
	// window.reset(new Golem::EditorWindow());
	window = std::make_shared<Golem::EditorWindow>();
	return success;
}

void close()
{

	// Quit SDL subsystems
	SDL_Quit();
}

int WinMain(int argc, char *args[])
{
	// Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		Golem::Profiler::GetInstance().StartSession("Golem", "Golem.json");
		window->init();
		window->open();
		window->gameLoop();
		Golem::Profiler::GetInstance().EndSession();
	}

	// Free resources and close SDL
	close();

	return 0;
}
