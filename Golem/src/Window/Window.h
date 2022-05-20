/*
 * Window.h
 *
 *  Created on: 01-May-2022
 *      Author: anantha
 */

#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <SDL2/SDL.h>
#include <string>

#include <Core/Gui/GuiManager.h>
#include <SOIL/image_dxt.h>
#include <memory>		   // remove this
#include <Vulkan/Vulkan.h> // remove this
#include <SDL2/SDL_vulkan.h>

#include <Core/Time/Timer.h>

enum GRAPHICS_LIBRARY
{
	OPENGL,
	VULKAN,
};

class Window
{

	std::string title = "Unnamed Window";
	int m_width = 1000;
	int m_height = 720;
	int m_minWidth = 320;
	int m_minHeight = 320;

public:
	Window();
	virtual ~Window();

	// Singleton instance.
	static Window *Instance;
	static inline Window *getWindow() { return Instance; }
	inline SDL_Window *getSDLWindow() { return m_sdlWindow; }

	static GRAPHICS_LIBRARY GraphicsLibrary;
	static inline void setGraphicsLibrary(GRAPHICS_LIBRARY library)
	{
		GraphicsLibrary = library;
	}

	void init();
	void initGL();
	int initVulkan();
	void open();
	void close();

	virtual void onInit(){};
	virtual void onOpen(){};
	virtual void onClose(){};
	virtual void onUpdate(){};
	virtual void onEvent(SDL_Event &e){};
	virtual void onGui(){};
	virtual void onRender(){};

	// Focuses on window.
	void focus();

	// Shows windows contents.
	virtual void render();
	virtual void renderGL();
	virtual void renderVulkan();
	// Updates windows contents.
	virtual void update();

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
	void cleanGL();
	void cleanVulkan();
	SDL_Renderer *m_sdlRenderer = nullptr;
	bool m_show_demo_window = true;

private:
	SDL_Window *m_sdlWindow = nullptr;
	unsigned int m_windowId = 0;

	SDL_GLContext m_context = nullptr; // Graphics Processor.

	unsigned int m_enabled_layer_count = 1;
	const char *const *m_extension_names = nullptr;
	VkInstance m_vulkanInstance = nullptr;
	unsigned int m_enabled_extension_count = 0;
	VkPhysicalDevice gpu = VK_NULL_HANDLE;
	SDL_vulkanSurface m_vulkanSurface;
	// const char gAppName[] = "VulkanDemo";
	// const char gEngineName[] = "VulkanDemoEngine";

	int m_interval = 1; // VSync.

	Uint32 m_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	bool m_cleaned = false;
	bool m_closed = false;

	bool m_shown = false;
	bool m_minimized = false;
	bool m_mouseFocus = false;
	bool m_keyboardFocus = false;

	// Tracker m_tracker;
	// std::shared_ptr<GuiWindow> guiWindow;
};

#endif /* WINDOW_WINDOW_H_ */
