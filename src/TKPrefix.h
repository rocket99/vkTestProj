//
//
#ifndef __TK_Prefix_h__
#define __TK_Prefix_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <libpng/png.h>
#include <jpeglib.h>
	
#ifdef __cplusplus
}
#endif
    
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>

//#define NDEBUG
#include <assert.h>
#include <pthread.h>
using namespace std;

#include <jsoncpp/json/json.h>
#define VK_USE_PLATFORM_XCB_KHR
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>

#define TK_LOG_ENABLED  1
#if  TK_LOG_ENABLED
#define TKLog(format, ...)  printf("%s[%d]\n    %s: " format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define TKLog(format, ...)
#endif

#define TK_CHECK(value) if(value != VK_SUCCESS){ \
        printf("%s(%s)[%d]:error occured eoorNo.%d\n", \
               __FILE__, __FUNCTION__, __LINE__, value);    \
    }
#define SCREEN_WIDTH   1000
#define SCREEN_HEIGHT  1000
#define PNG_BYTES_TO_CHECK 4
#define PROJECT_PATH   "/home/sky/Github/vkTestProj"

struct BaseVkInfo {
    VkInstance       instance           = VK_NULL_HANDLE;
    VkDevice         device             = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice     = VK_NULL_HANDLE;
    VkSurfaceKHR     surface            = VK_NULL_HANDLE;
    
    VkQueue          graphicsQueue      = VK_NULL_HANDLE;
    VkQueue          presentQueue       = VK_NULL_HANDLE;
    uint32_t         graphicsQueueFamily  = UINT32_MAX;
    uint32_t         presentQueueFamily   = UINT32_MAX;
    uint32_t         colorAttachmentCount = UINT32_MAX;
    
    VkSwapchainKHR   swapchain          = VK_NULL_HANDLE;
    VkCommandPool    commandPool        = VK_NULL_HANDLE;
    VkRenderPass     renderPass         = VK_NULL_HANDLE;

    VkImageView      depthImageview     = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool     = VK_NULL_HANDLE;

    VkPhysicalDeviceFeatures         physicalDeviceFeatures;
    VkPhysicalDeviceMemoryProperties physicalMemoryInfo;

    std::vector<VkSurfaceFormatKHR>  surfaceFormats;
    std::vector<VkImage>             swapchainImages;
    std::vector<VkImageView>         swapchainImageViews;
    std::vector<VkFramebuffer>       framebuffers;
    std::vector<VkCommandBuffer>     commandBuffers;

    std::vector<VkFence>             fences;
    std::vector<VkSemaphore>         graphicsSemaphore;
    std::vector<VkSemaphore>         presentSemaphore;
};

#endif
