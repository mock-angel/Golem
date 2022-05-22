/*
 * Window.cpp
 *
 *  Created on: 01-May-2022
 *      Author: sujak
 */

#include <iostream>

#include "Window.h"
#include "VulkanWindowUtils.h"

#include <vulkan/vulkan.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../Core/GUI/Gui.h"
#include <Core/Time/Timer.h>
#include <Core/Debug/Debug.h>
#include <Core/Tracker/Tracker.h>

#include <imgui.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_vulkan.h"
#include <SDL2/SDL_vulkan.h>
#include <string>
#include <set>
#include <vector>

namespace Golem
{

	Window *Window::Instance = nullptr;
	GRAPHICS_LIBRARY Window::GraphicsLibrary = OPENGL;
	static bool g_SwapChainRebuild = false;

	void GLAPIENTRY
	MessageCallback(GLenum source,
					GLenum type,
					GLuint id,
					GLenum severity,
					GLsizei length,
					const GLchar *message,
					const void *userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
				type, severity, message);
	}

	// VkResult vkCreateInstance(
	// 	const VkInstanceCreateInfo *pCreateInfo,
	// 	const VkAllocationCallbacks *pAllocator,
	// 	VkInstance *instance)
	// {

	// 	if (pCreateInfo == nullptr || instance == nullptr)
	// 	{
	// 		// log("Null pointer passed to required parameter!");
	// 		return VK_ERROR_INITIALIZATION_FAILED;
	// 	}

	// 	//    return real_vkCreateInstance(pCreateInfo, pAllocator, instance);
	// }
	static void check_vk_result(VkResult err)
	{
		if (err == 0)
			return;
		printf("[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}

	static VkDevice g_Device = VK_NULL_HANDLE;
	static VkQueue g_Queue = VK_NULL_HANDLE;
	ImGui_ImplVulkanH_Window *wd;
	static void FrameRender(ImGui_ImplVulkanH_Window *wd, ImDrawData *draw_data)
	{
		VkResult err;

		VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
		VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
		err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		{
			g_SwapChainRebuild = true;
			return;
		}
		check_vk_result(err);

		ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];
		{
			err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking
			check_vk_result(err);

			err = vkResetFences(g_Device, 1, &fd->Fence);
			check_vk_result(err);
		}
		{
			err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
			check_vk_result(err);
			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
			check_vk_result(err);
		}
		{
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = wd->RenderPass;
			info.framebuffer = fd->Framebuffer;
			info.renderArea.extent.width = wd->Width;
			info.renderArea.extent.height = wd->Height;
			info.clearValueCount = 1;
			info.pClearValues = &wd->ClearValue;
			vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}

		// Record dear imgui primitives into command buffer
		ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

		// Submit command buffer
		vkCmdEndRenderPass(fd->CommandBuffer);
		{
			VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			VkSubmitInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &image_acquired_semaphore;
			info.pWaitDstStageMask = &wait_stage;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &fd->CommandBuffer;
			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = &render_complete_semaphore;

			err = vkEndCommandBuffer(fd->CommandBuffer);
			check_vk_result(err);
			err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
			check_vk_result(err);
		}
	}

	static void FramePresent(ImGui_ImplVulkanH_Window *wd)
	{
		if (g_SwapChainRebuild)
			return;
		VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
		VkPresentInfoKHR info = {};
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &render_complete_semaphore;
		info.swapchainCount = 1;
		info.pSwapchains = &wd->Swapchain;
		info.pImageIndices = &wd->FrameIndex;
		VkResult err = vkQueuePresentKHR(g_Queue, &info);
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		{
			g_SwapChainRebuild = true;
			return;
		}
		check_vk_result(err);
		wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
	}

	Window::Window()
	{
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
		if (GraphicsLibrary == OPENGL)
		{
			initGL();
		}
		else if (GraphicsLibrary == VULKAN)
		{
			initVulkan();
		}
		else
		{
			std::cout << "Window::Graphics library not set.\n";
		}

		onInit();
	}

	void Window::initGL()
	{
		// FIXME: This shouldn't be here, should be in application etc.
		// glewInit();
		m_sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, m_flags | SDL_WINDOW_OPENGL);

		if (m_sdlWindow == nullptr)
		{
			std::cout << SDL_GetError();
			return;
		}

		m_windowId = SDL_GetWindowID(m_sdlWindow);

		SDL_SetWindowMinimumSize(m_sdlWindow, m_minWidth, m_minHeight);

		hide();

		m_context = SDL_GL_CreateContext(m_sdlWindow);
		int ver = 0;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &ver);
		std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << ver << std::endl;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &ver);
		std::cout << "SDL_GL_CONTEXT_MINOR_VERSION : " << ver << std::endl;
		SDL_GL_SetSwapInterval(m_interval);

		glewInit();

		Gui::InitForGL(m_sdlWindow, m_context);

		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		std::cout << "init" << std::endl;
		//	glDebugMessageCallback(MessageCallback, 0);

		show();

		// // Initialize Projection Matrix
		// glMatrixMode(GL_PROJECTION);
		// glLoadIdentity();

		// // Initialize Modelview Matrix
		// glMatrixMode(GL_MODELVIEW);
		// glLoadIdentity();

		glClearColor(0.f, 0.f, 0.f, 1.f);

		// Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			// printf("Error initializing OpenGL! %s\n", gluErrorString(error));
			return;
		}
	}

	int Window::initVulkan()
	{
		m_sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, m_flags | SDL_WINDOW_VULKAN);

		if (m_sdlWindow == nullptr)
		{
			std::cout << SDL_GetError();
			return -1;
		}

		m_windowId = SDL_GetWindowID(m_sdlWindow);

		SDL_SetWindowMinimumSize(m_sdlWindow, m_minWidth, m_minHeight);

		SDL_Vulkan_LoadLibrary("C:\\Windows\\SysWOW64\\vulkan-1.dll");
		// Get available vulkan extensions, necessary for interfacing with native window
		// SDL takes care of this call and returns, next to the default VK_KHR_surface a platform specific extension
		// When initializing the vulkan instance these extensions have to be enabled in order to create a valid
		// surface later on.
		std::vector<std::string> found_extensions;
		// if (!getAvailableVulkanExtensions(m_sdlWindow, found_extensions))
		// 	return -1;
		getAvailableVulkanExtensions(m_sdlWindow, found_extensions);

		// Get available vulkan layer extensions, notify when not all could be found
		std::vector<std::string> found_layers;
		if (!getAvailableVulkanLayers(found_layers))
			return -1;

		// Warn when not all requested layers could be found
		if (found_layers.size() != getRequestedLayerNames().size())
			std::cout << "warning! not all requested layers could be found!\n";

		// Create Vulkan Instance
		VkInstance &instance = m_vulkanInstance;
		if (!createVulkanInstance(found_layers, found_extensions, instance))
			return -1;

		// Vulkan messaging callback
		VkDebugReportCallbackEXT callback;
		setupDebugCallback(instance, callback);

		// Select GPU after succsessful creation of a vulkan instance (jeeeej no global states anymore)
		VkPhysicalDevice gpu;
		unsigned int graphics_queue_index(-1);
		if (!selectGPU(instance, gpu, graphics_queue_index))
			return -1;

		// Create a logical device that interfaces with the physical device
		VkDevice device;
		if (!createLogicalDevice(gpu, graphics_queue_index, found_layers, device))
			return -1;

		// Create the surface we want to render to, associated with the window we created before
		// This call also checks if the created surface is compatible with the previously selected physical device and associated render queue
		// VkSurfaceKHR presentation_surface;
		if (!createSurface(m_sdlWindow, instance, gpu, graphics_queue_index, m_vulkanSurface))
			return -1;

		// Create swap chain
		VkSwapchainKHR swap_chain = NULL;
		if (!createSwapChain(m_vulkanSurface, gpu, device, swap_chain))
			return -1;

		// Get image handles from swap chain
		std::vector<VkImage> chain_images;
		if (!getSwapChainImageHandles(device, swap_chain, chain_images))
			return -1;

		// Fetch the queue we want to submit the actual commands to
		VkQueue graphics_queue;
		getDeviceQueue(device, graphics_queue_index, graphics_queue);

		std::cout << "\nsuccessfully initialized vulkan and physical device (gpu).\n";
		std::cout << "successfully created a window and compatible surface\n";
		std::cout << "successfully created swapchain\n";
		std::cout << "ready to render!\n";
		printf("\n");
		// if (!SDL_Vulkan_CreateSurface(m_sdlWindow, m_vulkanInstance, &m_vulkanSurface))
		// {
		// 	std::cout << "failed to create surface\n";
		// 	return -1;
		// }
		VkResult err;
		static ImGui_ImplVulkanH_Window g_MainWindowData;

		wd = &g_MainWindowData;
		// SetupVulkanWindow(wd, m_vulkanSurface, m_vulkanInstance, m_sdlWindow);
		wd->Surface = m_vulkanSurface;
		const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

		wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(gpu, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
#ifdef IMGUI_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
#else
		VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
		wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(gpu, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		wd->Swapchain = swap_chain;
		std::cout << "gpu: " << gpu << " device: " << device << " graphics_queue_index: " << graphics_queue_index << std::endl;
		;

		// Create SwapChain, RenderPass, Framebuffer, etc.
		// IM_ASSERT(g_MinImageCount >= 2);
		ImGui_ImplVulkanH_CreateOrResizeWindow(m_vulkanInstance, gpu, device, wd, graphics_queue_index, nullptr, getWidth(), getHeight(), 1);
		std::cout << "test1" << std::endl;

		static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
		{
			VkDescriptorPoolSize pool_sizes[] =
				{
					{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
					{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
					{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
					{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
					{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
					{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
					{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
			pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;
			err = vkCreateDescriptorPool(device, &pool_info, nullptr, &g_DescriptorPool);
			check_vk_result(err);
		}
		std::cout << "test" << std::endl;

		IMGUI_CHECKVERSION();
		m_context = ImGui::CreateContext();

		ImGui_ImplSDL2_InitForVulkan(m_sdlWindow);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_vulkanInstance;
		init_info.PhysicalDevice = gpu;
		init_info.Device = device;
		init_info.QueueFamily = graphics_queue_index;
		init_info.Queue = graphics_queue;
		// init_info.PipelineCache = g_PipelineCache;
		init_info.DescriptorPool = g_DescriptorPool;
		init_info.Subpass = 0;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		// init_info.Allocator = g_Allocator;
		init_info.CheckVkResultFn = check_vk_result;
		std::cout << "test" << std::endl;

		std::cout << std::endl;
		ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
		// Upload Fonts
		{
			// Use any command queue
			VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
			VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

			err = vkResetCommandPool(device, command_pool, 0);
			check_vk_result(err);
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(command_buffer, &begin_info);
			check_vk_result(err);
			std::cout << std::endl;
			ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

			VkSubmitInfo end_info = {};
			end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			end_info.commandBufferCount = 1;
			end_info.pCommandBuffers = &command_buffer;
			err = vkEndCommandBuffer(command_buffer);
			check_vk_result(err);
			err = vkQueueSubmit(graphics_queue, 1, &end_info, VK_NULL_HANDLE);
			check_vk_result(err);

			err = vkDeviceWaitIdle(device);
			check_vk_result(err);
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}

		g_Device = device;
		g_Queue = graphics_queue;
		// Gui::InitForVulkan(m_sdlWindow);
	}

	void Window::open()
	{
		show();
		awake();

		onOpen();
		// m_guiManager.addGuiWindow(std::make_shared<GuiWindow>());
	}
	void Window::close()
	{
		SDL_DestroyWindow(m_sdlWindow);
		m_sdlWindow = nullptr;

		clean();

		onClose();
	}

	void Window::focus()
	{
		SDL_RaiseWindow(m_sdlWindow);
	}
	void Window::render()
	{
		if (GraphicsLibrary == VULKAN)
			renderVulkan();
		else
			renderGL();
	}
	void Window::renderGL()
	{
		// Golem::Timer timer("Window::renderGL");
		GOLEM_PROFILE_FUNCTION();

		SDL_GL_MakeCurrent(this->m_sdlWindow, this->m_context);
		glViewport(0, 0, m_width, m_height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// bool show_demo_window = false;

		// gui new frame
		Gui::NewFrame();
		// m_guiManager.onGui();

		{
			glClearColor(.3, .1, .2, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			onGui();

			if (m_show_demo_window)
				ImGui::ShowDemoWindow(&m_show_demo_window);

			ImGui::Render();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			if (m_shown && !m_minimized)
			{
				// render_sequence();
				onRender();

				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			static int prevWidth, prevHeight;

			if (prevWidth != m_width || prevHeight != m_height)
			{
				// gameCam->resizeTextures();

				prevWidth = m_width;
				prevHeight = m_height;
			}
			// glColor3f(0, 0, 0);
			// glBegin(GL_LINES);
			// glVertex2f(-0.5f, -0.5f);
			// glVertex2f(0.5f, -0.5f);
			// glVertex2f(0.5f, 0.5f);
			// glVertex2f(-0.5f, 0.5f);
			// glEnd();
			// glFlush();
		}

		SDL_GL_SwapWindow(m_sdlWindow);
	}
	void Window::renderVulkan()
	{
		Golem::Timer timer("Window::renderVulkan");

		// Gui::NewFrame();
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		if (m_show_demo_window)
			ImGui::ShowDemoWindow(&m_show_demo_window);

		onGui();

		// Rendering
		ImGui::Render();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		ImDrawData *draw_data = ImGui::GetDrawData();

		if (m_shown && !m_minimized)
		{
			// render_sequence();
			const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
			if (!is_minimized)
			{
				// render_sequence();
				onRender();
				// wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
				// wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
				// wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
				// wd->ClearValue.color.float32[3] = clear_color.w;
				FrameRender(wd, draw_data);
				FramePresent(wd);
			}
		}

		static int prevWidth, prevHeight;

		if (prevWidth != m_width || prevHeight != m_height)
		{
			// gameCam->resizeTextures();

			prevWidth = m_width;
			prevHeight = m_height;
		}
	}
	// Updates windows contents.
	void Window::update()
	{
		onUpdate();
	}

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

	void Window::clean()
	{
		m_cleaned = true;

		if (GraphicsLibrary == VULKAN)
			cleanVulkan();
		else
			cleanGL();
	}

	void Window::cleanVulkan()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplSDL2_Shutdown();
	}

	void Window::cleanGL()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
	}

	void Window::gameLoop()
	{
		std::cout << "Window::GameLoop() started" << std::endl;

		// start subsystems.

		// start game loop.

		while (!m_closed)
		{
			// Tracker::GetInstance().NewFrame();

			// Process Events
			processEvents();

			// update
			update();

			// render
			render();
		}

		clean();
	}

	void Window::processEvents()
	{
		SDL_Event e;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_WINDOWEVENT)
			{
				handleWindowEvent(e);
			}
			onEvent(e);

			// m_guiManager.handleEvent(e);
		}
	}

	void Window::handleWindowEvent(SDL_Event &e)
	{
		// TODO: remove unnecessary window ID check.

		// If an event was detected for this window
		if (e.type == SDL_WINDOWEVENT && e.window.windowID == m_windowId) // && e.window.windowID == m_windowId)
		// TODO: Avoid unnecessarity searching and calling for events from window.
		{
			// Caption update flag.
			// bool updateCaption = false;

			switch (e.window.event)
			{
			// Window appeared.
			case SDL_WINDOWEVENT_SHOWN:
				m_shown = true;
				std::cout << "W: Show" << std::endl;
				break;

			// Window disappeared.
			case SDL_WINDOWEVENT_HIDDEN:
				m_shown = false;
				std::cout << ("W: Hide") << std::endl;
				break;

			// Get new dimensions and repaint.
			case SDL_WINDOWEVENT_SIZE_CHANGED:

				m_width = e.window.data1;
				m_height = e.window.data2;
				std::cout << ("W: Resize") << std::endl;

				// SDL_RenderPresent( m_renderer );
				// TODO: Rewrite above line for surface render...
				glViewport(0, 0, m_width, m_height);

				break;
			// Repaint on expose.
			case SDL_WINDOWEVENT_EXPOSED:
				// SDL_RenderPresent(m_renderer);

				std::cout << "W: Exposed" << std::endl;
				break;

			// Mouse enter.
			case SDL_WINDOWEVENT_ENTER:
				m_mouseFocus = true;

				std::cout << "M: Enter"
						  << std::endl;
				break;

			// Mouse exit.
			case SDL_WINDOWEVENT_LEAVE:
				m_mouseFocus = false;

				std::cout << "M: Leave"
						  << std::endl;
				break;

			// Keyboard focus gained.
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				m_keyboardFocus = true;

				std::cout << "K: focus Gained"
						  << std::endl;
				break;

			// Keyboard focus lost.
			case SDL_WINDOWEVENT_FOCUS_LOST:
				m_keyboardFocus = false;

				std::cout << "W: focus Lost"
						  << std::endl;
				break;

			// Window minimized.
			case SDL_WINDOWEVENT_MINIMIZED:
				m_minimized = true;

				std::cout << "W: MINIMIZED"
						  << std::endl;
				break;

			// Window maxized.
			case SDL_WINDOWEVENT_MAXIMIZED:
				m_minimized = false;
				std::cout << "W: MAXIMIZED"
						  << std::endl;
				break;

			// Window restored.
			case SDL_WINDOWEVENT_RESTORED:
				m_minimized = false;

				std::cout << "W: RESTORED" << std::endl;
				break;

			// Hide on close.
			case SDL_WINDOWEVENT_CLOSE:
				SDL_HideWindow(m_sdlWindow);

				SDL_DestroyRenderer(m_sdlRenderer);
				SDL_DestroyWindow(m_sdlWindow);

				m_sdlWindow = NULL;
				m_sdlRenderer = NULL;
				m_closed = true;
				break;
			}
		}
	}
}