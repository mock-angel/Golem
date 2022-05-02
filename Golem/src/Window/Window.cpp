/*
 * Window.cpp
 *
 *  Created on: 01-May-2022
 *      Author: sujak
 */

#include <iostream>

#include "Window.h"

// #include <GL/glew.h>

Window *Window::Instance = nullptr;

Window::Window()
{
	// TODO Auto-generated constructor stub
	if (Instance != nullptr)
	{
		std::cout << "Window already exists.\n";
		return;
	}
	else
	{
		Instance = this;
	}
}

Window::~Window() {}

void Window::init()
{
	// FIXME: This shouldn't be here, should be in application etc.
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << SDL_GetError();
		return;
	}

	m_sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, m_flags);

	if (m_sdlWindow == nullptr)
	{
		std::cout << SDL_GetError();
		return;
	}

	m_windowId = SDL_GetWindowID(m_sdlWindow);

	SDL_SetWindowMinimumSize(m_sdlWindow, m_minWidth, m_minHeight);

	hide();

	// SDL attributes, must be set before context creation.
	// FIXME: This shouldn't be here, should be in application etc?
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_context = SDL_GL_CreateContext(m_sdlWindow);

	SDL_GL_SetSwapInterval(m_interval);

	// glewInit();
	// During init, enable debug output
	// glEnable(GL_DEBUG_OUTPUT);
	// glDebugMessageCallback(MessageCallback, 0);
}
void Window::open()
{
	show();
	awake();
}
void Window::close()
{
	SDL_DestroyWindow(m_sdlWindow);
	m_sdlWindow = nullptr;

	clean();
}

void Window::focus()
{
	SDL_RaiseWindow(m_sdlWindow);
}

void Window::render()
{
	;
}

// Updates windows contents.
void Window::update() {}

// Window dimensions.
int Window::getWidth()
{
	return m_width;
}
int Window::getHeight()
{
	return m_height;
}

// bool Window::hasMouseFocus() { ; }
// bool Window::hasKeyboardFocus() { ; }
// bool Window::isMinimized() { ; }
// bool Window::isShown() { ; }
// bool Window::isClosed() { ; }

void Window::hide() { SDL_HideWindow(m_sdlWindow); }
void Window::show() { SDL_ShowWindow(m_sdlWindow); }
void Window::maximise() { SDL_MaximizeWindow(m_sdlWindow); }
void Window::minimise() { SDL_MinimizeWindow(m_sdlWindow); }
void Window::restore() { SDL_RestoreWindow(m_sdlWindow); }

void Window::awake() { std::cout << "Calling Window::Awake().\n"; }

void Window::clean() { m_cleaned = true; }