#include <vulkan/vulkan.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../Core/GUI/Gui.h"
#include <imgui.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include "imgui/backends/imgui_impl_opengl3.h"
#include <SDL2/SDL_vulkan.h>
#include <string>
#include <set>
#include <vector>

const std::set<std::string> &getRequestedLayerNames();
const std::set<std::string> &getRequestedDeviceExtensionNames();
const std::vector<VkImageUsageFlags> getRequestedImageUsages();
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
                                                    uint64_t obj,
                                                    size_t location,
                                                    int32_t code,
                                                    const char *layerPrefix,
                                                    const char *msg,
                                                    void *userData);
VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);
bool setupDebugCallback(VkInstance instance, VkDebugReportCallbackEXT &callback);
void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator);
bool getAvailableVulkanLayers(std::vector<std::string> &outLayers);
bool getAvailableVulkanExtensions(SDL_Window *window, std::vector<std::string> &outExtensions);
bool createVulkanInstance(const std::vector<std::string> &layerNames, const std::vector<std::string> &extensionNames, VkInstance &outInstance);
bool selectGPU(VkInstance instance, VkPhysicalDevice &outDevice, unsigned int &outQueueFamilyIndex);
bool createLogicalDevice(VkPhysicalDevice &physicalDevice,
                         unsigned int queueFamilyIndex,
                         const std::vector<std::string> &layerNames,
                         VkDevice &outDevice);
void getDeviceQueue(VkDevice device, int familyQueueIndex, VkQueue &outGraphicsQueue);
bool createSurface(SDL_Window *window, VkInstance instance, VkPhysicalDevice gpu, uint32_t graphicsFamilyQueueIndex, VkSurfaceKHR &outSurface);
bool getPresentationMode(VkSurfaceKHR surface, VkPhysicalDevice device, VkPresentModeKHR &ioMode);
bool getSurfaceProperties(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR &capabilities);
unsigned int getNumberOfSwapImages(const VkSurfaceCapabilitiesKHR &capabilities);
VkExtent2D getSwapImageSize(const VkSurfaceCapabilitiesKHR &capabilities);
bool getImageUsage(const VkSurfaceCapabilitiesKHR &capabilities, VkImageUsageFlags &outUsage);
VkSurfaceTransformFlagBitsKHR getTransform(const VkSurfaceCapabilitiesKHR &capabilities);
bool getFormat(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR &outFormat);
bool createSwapChain(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device, VkSwapchainKHR &outSwapChain);
bool getSwapChainImageHandles(VkDevice device, VkSwapchainKHR chain, std::vector<VkImage> &outImageHandles);
SDL_Window *createWindow();
void quit(VkInstance instance, VkDevice device, VkDebugReportCallbackEXT callback, VkSwapchainKHR chain, VkSurfaceKHR presentation_surface);
