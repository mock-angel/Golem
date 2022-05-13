//============================================================================
// Name        : Golem.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
//#include <SDL2/SDL.h>
#include <memory>

#include "Window/Window.h"
using namespace std;

// Frees media and shuts down SDL
void close();

std::shared_ptr<Window> window = NULL;
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

	window = std::make_shared<Window>();
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
		window->init();
		window->open();
		window->gameLoop();
	}

	// Free resources and close SDL
	close();

	return 0;
}
