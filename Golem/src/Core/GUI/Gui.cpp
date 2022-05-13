/*
 * Gui.cpp
 *
 *  Created on: 06-May-2022
 *      Author: anantha
 */

#include "Gui.h"
#include <Window/Window.h>

#include <imgui.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

SDL_Window *Gui::sdl_window;
ImGuiContext *Gui::imgui_context;

Gui::Gui()
{
	// TODO Auto-generated constructor stub
}

Gui::~Gui()
{
	// TODO Auto-generated destructor stub
}

void Gui::Init(SDL_Window *t_sdlwindow)
{
	IMGUI_CHECKVERSION();
	imgui_context = ImGui::CreateContext();

	// code from : https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
														  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
														  //	 io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	// window is the SDL_Window*
	// context is the SDL_GLContext

	sdl_window = t_sdlwindow;
}

void Gui::InitForGL(SDL_Window *t_sdlwindow, SDL_GLContext context)
{
	Gui::Init(t_sdlwindow);

	// Setup Platform/Renderer bindings
	// window is the SDL_Window*
	// context is the SDL_GLContext
	ImGui_ImplSDL2_InitForOpenGL(t_sdlwindow, context);
	ImGui_ImplOpenGL3_Init();

	sdl_window = t_sdlwindow;
}

void Gui::InitForVulkan(SDL_Window *t_sdlwindow)
{
	Gui::Init(t_sdlwindow);
	ImGui_ImplSDL2_InitForVulkan(t_sdlwindow);

		// 1: create descriptor pool for IMGUI
	//  the size of the pool is very oversize, but it's copied from imgui demo itself.
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
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	// VK_CHECK(vkCreateDescriptorPool(_device, &pool_info, nullptr, &imguiPool));

	// // Setup Platform/Renderer bindings
	// // window is the SDL_Window*
	// // context is the SDL_GLContext

	// ImGui_ImplSDL2_InitForVulkan(t_sdlwindow);
	// // this initializes imgui for Vulkan
	// ImGui_ImplVulkan_InitInfo init_info = {};
	// init_info.Instance = vulkanInstance;
	// init_info.PhysicalDevice = _chosenGPU;
	// init_info.Device = _device;
	// init_info.Queue = _graphicsQueue;
	// init_info.DescriptorPool = imguiPool;
	// init_info.MinImageCount = 3;
	// init_info.ImageCount = 3;
	// init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	// ImGui_ImplVulkan_Init(&init_info, _renderPass);
	// // immediate_submit([&](VkCommandBuffer cmd)
	// // 				 { ImGui_ImplVulkan_CreateFontsTexture(cmd); });

	// // clear font textures from cpu data
	// ImGui_ImplVulkan_DestroyFontUploadObjects();

	// // add the destroy the imgui created structures
	// // _mainDeletionQueue.push_function([=]()
	// // 								 {

	// // 	vkDestroyDescriptorPool(_device, imguiPool, nullptr);
	// // 	ImGui_ImplVulkan_Shutdown(); });
}

void Gui::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------

// Demonstrate creating a "main" fullscreen menu bar and populating it.
// Note the difference between BeginMainMenuBar() and BeginMenuBar():
// - BeginMenuBar() = menu-bar inside current window (which needs the ImGuiWindowFlags_MenuBar flag!)
// - BeginMainMenuBar() = helper to create menu-bar-sized window at the top of the main viewport + call BeginMenuBar() into it.
static void ShowMenuFile();
static void ShowExampleAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Project"))
		{
			ShowMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z"))
			{
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
			{
			} // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X"))
			{
			}
			if (ImGui::MenuItem("Copy", "CTRL+C"))
			{
			}
			if (ImGui::MenuItem("Paste", "CTRL+V"))
			{
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

// Note that shortcuts are currently provided for display only
// (future version will add explicit flags to BeginMenu() to request processing shortcuts)
static void ShowMenuFile()
{
	ImGui::MenuItem("Project Options", NULL, false, false);
	if (ImGui::MenuItem("New Project"))
	{
	}
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
	}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
	}
	if (ImGui::MenuItem("Save As.."))
	{
	}

	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char *name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}

	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		static bool b = true;
		ImGui::Checkbox("SomeOption", &b);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		// IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true))
	{
	}
	if (ImGui::MenuItem("Quit"))
	{
		// Quit
	}
}

// void Gui::RenderGUI(){
//     ShowExampleAppMainMenuBar();
// }
void ShowGui()
{
	ShowExampleAppMainMenuBar();
}

void Gui::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(sdl_window);
	ImGui::NewFrame();
}
