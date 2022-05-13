/*
 * Window.h
 *
 *  Created on: 01-May-2022
 *      Author: sujak
 */

#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <SDL2/SDL.h>
#include <string>

#include "../Gui/GuiManager.h"

class Window
{

	std::string title = "Unnamed Window";
	int m_width = 400;
	int m_height = 400;
	int m_minWidth = 320;
	int m_minHeight = 320;

public:
	Window();
	virtual ~Window();

	// Singleton instance.
	static Window *Instance;
	static inline Window *getWindow() { return Instance; }

	void init();
	void open();
	void close();

	// Focuses on window.
	void focus();

	// Shows windows contents.
	void render();

	// Updates windows contents.
	void update();

	// Window dimensions.
	int getWidth();
	int getHeight();

	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();
	bool isShown();
	bool isClosed();

	void hide();
	void show();
	void maximise();
	void minimise();
	void restore();

	void gameLoop();
	void processEvents(); // TODO: Move this function to a event handler?
	void handleWindowEvent(SDL_Event &e);

	virtual void awake();

protected:
	void clean();
	SDL_Renderer *m_sdlRenderer = nullptr;

private:
	SDL_Window *m_sdlWindow = nullptr;
	unsigned int m_windowId = 0;
	SDL_GLContext m_context = nullptr; // Graphics Processor.

	int m_interval = 1; // VSync.

	Uint32 m_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

	bool m_cleaned = false;
	bool m_closed = false;

	bool m_shown = false;
	bool m_minimized = false;
	bool m_mouseFocus = false;
	bool m_keyboardFocus = false;

	GuiManager m_guiManager;
};

#endif /* WINDOW_WINDOW_H_ */
