//
//TKBaseVkInfo.cpp
//2018-12-13

#include "TKBaseInfo.h"

static TKBaseInfo *instance = nullptr;

TKBaseInfo::TKBaseInfo() {
    m_info = new BaseVkInfo;
}

TKBaseInfo::~TKBaseInfo() {
    delete m_renderPass;
    delete m_depthImageView;
    for(uint32_t i=0; i<m_info->framebuffers.size(); ++i){
        vkDestroyFramebuffer(m_info->device, m_info->framebuffers[i], nullptr);
    }
    for(uint32_t i=0; i<m_info->swapchainImageViews.size(); ++i){
        vkDestroyImageView(m_info->device, m_info->swapchainImageViews[i], nullptr);
    }

    for(uint32_t i=0; i<m_info->fences.size(); ++i){
        vkDestroyFence(m_info->device, m_info->fences[i], nullptr);
    }

    for(uint32_t i=0; i<m_info->graphicsSemaphore.size(); ++i){
        vkDestroySemaphore(m_info->device, m_info->graphicsSemaphore[i], nullptr);
    }

    for(uint32_t i=0; i<m_info->presentSemaphore.size(); ++i){
        vkDestroySemaphore(m_info->device, m_info->presentSemaphore[i], nullptr);
    }
    
    if(m_info->commandPool != VK_NULL_HANDLE){
        vkDestroyCommandPool(m_info->device, m_info->commandPool, nullptr);
    }

	if(m_info->descriptorPool != VK_NULL_HANDLE){
		vkDestroyDescriptorPool(m_info->device, m_info->descriptorPool, nullptr);
	}
	
    if(m_info->swapchain != VK_NULL_HANDLE){
        vkDestroySwapchainKHR(m_info->device, m_info->swapchain, nullptr);
    }
    
    if(m_info->surface != VK_NULL_HANDLE){
        vkDestroySurfaceKHR(m_info->instance, m_info->surface, nullptr);
    }    
    
    if(m_info->device != VK_NULL_HANDLE){
        vkDestroyDevice(m_info->device, nullptr);
    }
    
    if(m_info->instance != VK_NULL_HANDLE){
        vkDestroyInstance(m_info->instance, nullptr);
    }
	TKLog("destruct TKBaseInfo end mark!\n");
    delete m_info;
}

TKBaseInfo *TKBaseInfo::share() {
    if(instance == nullptr){
        instance = new TKBaseInfo();
    }
    return instance;
}

void TKBaseInfo::purge() {
    if(instance != nullptr){
        delete instance;
    }
    instance = nullptr;
}

BaseVkInfo *TKBaseInfo::Info() {
    return TKBaseInfo::share()->m_info;
}


void TKBaseInfo::enumeratePhysicalDevices(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_info->instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(m_info->instance, &deviceCount, physicalDevices.data());

    for(uint32_t i=0; i<physicalDevices.size(); ++i){
        VkPhysicalDeviceProperties property;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &property);
        
        TKLog("[%d]Device ID 0x%x\n", i, property.deviceID);
        TKLog("[%d]Driver Version: %d\n", i, property.driverVersion);
        TKLog("[%d]Device name: %s\n", i, property.deviceName);
        TKLog("[%d]Device type: %d\n", i, property.deviceType);
        TKLog("[%d]API version: %d.%d.%d\n",i, (property.apiVersion>>22)&0x3FF,
              (property.apiVersion>>12)&0x3FF, (property.apiVersion&0xFFF));
        TKLog("[%d]limit color attachments: %d\n", i, property.limits.maxColorAttachments);
        
        if(property.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            TKLog("gpu device %s\n", property.deviceName);
            m_info->physicalDevice = physicalDevices[i];
            break;
        }
    }
}

void TKBaseInfo::initDevice() {
    vkGetPhysicalDeviceMemoryProperties(m_info->physicalDevice, &m_info->physicalMemoryInfo);
    vkGetPhysicalDeviceFeatures(m_info->physicalDevice, &m_info->physicalDeviceFeatures);
    
    uint32_t deviceQueueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_info->physicalDevice, &deviceQueueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(deviceQueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_info->physicalDevice, &deviceQueueFamilyCount,
                                             queueFamilyProperties.data());
	
    VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = nullptr;
    deviceInfo.flags = 0;
    std::vector<const char *> layerNames = {
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_parameter_validation",
    };
    deviceInfo.enabledLayerCount = layerNames.size(); 
    deviceInfo.ppEnabledLayerNames = layerNames.data();
    std::vector<const char *>extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    deviceInfo.enabledExtensionCount = extensions.size();
    deviceInfo.ppEnabledExtensionNames = extensions.data();
    deviceInfo.pEnabledFeatures = nullptr;

    std::vector<VkDeviceQueueCreateInfo> deviceQueueInfo(1);
    if(m_info->graphicsQueueFamily != m_info->presentQueueFamily){
        deviceQueueInfo.resize(2);
        deviceQueueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueInfo[0].pNext = nullptr;
        deviceQueueInfo[0].flags = 0;
        deviceQueueInfo[0].queueFamilyIndex = m_info->graphicsQueueFamily;
        deviceQueueInfo[0].queueCount = 1;
        float p0 = 5e-1;
        deviceQueueInfo[0].pQueuePriorities = &p0;

        deviceQueueInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueInfo[1].pNext = nullptr;
        deviceQueueInfo[1].flags = 0;
        deviceQueueInfo[1].queueFamilyIndex = m_info->presentQueueFamily;
        deviceQueueInfo[1].queueCount = 1;
        float p1 = 5e-1;
        deviceQueueInfo[0].pQueuePriorities = &p1;
    }else{
        deviceQueueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueInfo[0].pNext = nullptr;
        deviceQueueInfo[0].flags = 0;
        deviceQueueInfo[0].queueFamilyIndex = m_info->graphicsQueueFamily;
        deviceQueueInfo[0].queueCount = 1;
        float p = 5e-1;
        deviceQueueInfo[0].pQueuePriorities = &p;
    }
    deviceInfo.queueCreateInfoCount = deviceQueueInfo.size();
    deviceInfo.pQueueCreateInfos    = deviceQueueInfo.data();
    
    VkResult result = vkCreateDevice(m_info->physicalDevice, &deviceInfo, nullptr, &m_info->device);
    if (result != VK_SUCCESS){
        TKLog("Failed createing logical device:%d\n", result);
        return;
    }

    vkGetDeviceQueue(m_info->device, m_info->graphicsQueueFamily, 0, &m_info->graphicsQueue);
    vkGetDeviceQueue(m_info->device, m_info->presentQueueFamily, 0, &m_info->presentQueue);
}

void TKBaseInfo::setGraphicsQueueIndex(){
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_info->physicalDevice, &queueFamilyCount,
                                             nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_info->physicalDevice, &queueFamilyCount,
                                             queueFamilyProperties.data());

    VkBool32 graphicsQueueFound = VK_FALSE;
    uint32_t graphicsQueueFamily = 0;
    for(uint32_t i=0; i<queueFamilyCount; ++i){
        if(queueFamilyProperties[i].queueCount>0 &&
           queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            graphicsQueueFamily = i;
            graphicsQueueFound = VK_TRUE;
            break;
        }
    }

    if(graphicsQueueFound == VK_TRUE){
        m_info->graphicsQueueFamily = graphicsQueueFamily;
    }
}

void TKBaseInfo::setPresentQueueIndex(){
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_info->physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_info->physicalDevice, &queueFamilyCount,
                                             queueFamilyProperties.data());
    VkBool32 *supportsPresent = (VkBool32*)malloc(sizeof(VkBool32)*queueFamilyCount);
    for(uint32_t i=0; i<queueFamilyCount; ++i){         
        vkGetPhysicalDeviceSurfaceSupportKHR(m_info->physicalDevice, i,
                                             m_info->surface, &supportsPresent[i]);
    }
    
    VkBool32 presentQueueFound = VK_FALSE;
    for(int i=0; i<queueFamilyCount;++i){
        if(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            if(supportsPresent[i] == VK_TRUE){
                presentQueueFound = VK_TRUE;
                m_info->presentQueueFamily = i;
                //printf("presentation queue family %u\n", m_info->presentQueueFamily);
                break;
            }
        }
    }
}

void TKBaseInfo::initSwapchain() {
    VkSurfaceCapabilitiesKHR capability;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_info->physicalDevice, m_info->surface, &capability);
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_info->physicalDevice, m_info->surface,
                                              &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_info->physicalDevice, m_info->surface,
                                              &presentModeCount, presentModes.data());
    
    VkBool32 isSupported = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_info->physicalDevice, m_info->presentQueueFamily,
                                         m_info->surface, &isSupported);
    if(isSupported == VK_FALSE){
        TKLog("unsupported surface!\n");
        return;
    }
    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext                 = nullptr;
    createInfo.flags                 = 0;
    createInfo.surface               = m_info->surface;
    createInfo.minImageCount         = capability.minImageCount+1;
    createInfo.imageFormat           = m_info->surfaceFormats[0].format;
    createInfo.imageColorSpace       = m_info->surfaceFormats[0].colorSpace;
    createInfo.imageExtent           = capability.currentExtent;
    createInfo.imageArrayLayers      = 1;
    createInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices   = nullptr;
    createInfo.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode           = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped               = VK_TRUE;
    createInfo.oldSwapchain          = VK_NULL_HANDLE;

    VkResult ret = vkCreateSwapchainKHR(m_info->device, &createInfo, nullptr, &m_info->swapchain);
    assert(ret == VK_SUCCESS);
    if(ret != VK_SUCCESS){
        TKLog("create swapchain failed %d\n", ret);
        return;
    }

    uint32_t imageCount;
    ret = vkGetSwapchainImagesKHR(m_info->device, m_info->swapchain, &imageCount, nullptr);
    m_info->swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_info->device, m_info->swapchain, &imageCount,
                            m_info->swapchainImages.data());
    m_info->swapchainImageViews.resize(imageCount);
    for (uint32_t i=0; i<imageCount; ++i) {
        VkImageViewCreateInfo createInfo;
        createInfo.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext        = nullptr;
        createInfo.flags        = 0;
        createInfo.image        = m_info->swapchainImages[i];
        createInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format       = m_info->surfaceFormats[0].format;
   
        createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel   = 0;
        createInfo.subresourceRange.levelCount     = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount     = 1;
        vkCreateImageView(m_info->device, &createInfo, nullptr, &m_info->swapchainImageViews[i]);
    }

    m_depthImageView = TKImageView::createDepthStencilImage(m_info);
    m_info->depthImageview = m_depthImageView->imageView();
}

void TKBaseInfo::initFramebuffers(){
    VkFramebufferCreateInfo info;
    info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext           = nullptr;
    info.flags           = 0;
    info.renderPass      = m_info->renderPass;
    info.width           = SCREEN_WIDTH;
    info.height          = SCREEN_HEIGHT;
    info.layers          = 1;
    uint32_t colorAttachCount = m_info->colorAttachmentCount;
	TKLog("color attachment count %u\n", colorAttachCount);
    std::vector<VkImageView> attachments(colorAttachCount+1);
    m_info->framebuffers.resize(m_info->swapchainImageViews.size());
    for(uint32_t i=0; i<m_info->swapchainImageViews.size();++i){
        for(uint32_t j=0; j<colorAttachCount; ++j){
            attachments[j] = m_info->swapchainImageViews[i];
        }
        attachments[colorAttachCount] = m_info->depthImageview;
        info.attachmentCount  = attachments.size();
        info.pAttachments     = attachments.data();
        int ret = vkCreateFramebuffer(m_info->device, &info, nullptr, &m_info->framebuffers[i]);
        if(ret != VK_SUCCESS){
            TKLog("create frame buffer [%d] failed!\n", i);
        }
    }
}

void TKBaseInfo::initCommandPool() {
    VkCommandPoolCreateInfo cmdPoolCreateInfo;
    cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolCreateInfo.pNext = nullptr;
    cmdPoolCreateInfo.flags =
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolCreateInfo.queueFamilyIndex = m_info->graphicsQueueFamily;
    VkResult ret = vkCreateCommandPool(m_info->device, &cmdPoolCreateInfo,
                                       nullptr, &m_info->commandPool);
    if (ret != VK_SUCCESS) {
        printf("create command pool failed!\n");
        return;
    }
    uint32_t count = m_info->swapchainImages.size();
    m_info->commandBuffers.resize(count);
    VkCommandBufferAllocateInfo allocInfo;
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext              = nullptr;
    allocInfo.commandPool        = m_info->commandPool;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = count;
    ret = vkAllocateCommandBuffers(m_info->device, &allocInfo,
                                   m_info->commandBuffers.data());
    if(ret != VK_SUCCESS){
        TKLog("allocate command buffers error occured!\n");
    }
}

void TKBaseInfo::initRenderPass(){
	m_renderPass = TKRenderPass::createWithJson("renderpass.json");
	m_info->renderPass = m_renderPass->renderPass();
	//render pass has included depth attachment 
    m_info->colorAttachmentCount = m_renderPass->ColorAttachCount();
	
}

void TKBaseInfo::initFencesAndSemaphores(){
    uint32_t count = m_info->framebuffers.size();
    m_info->fences.resize(count);
    for(uint32_t i=0; i<count; ++i){
        VkFenceCreateInfo fenceInfo;
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.pNext = nullptr;
        fenceInfo.flags = 0;
        vkCreateFence(m_info->device, &fenceInfo, nullptr, &m_info->fences[i]);
    }
    m_info->graphicsSemaphore.resize(count);
    for(uint32_t i=0; i<count; ++i){
        VkSemaphoreCreateInfo semaphoreInfo;
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.pNext = nullptr;
        semaphoreInfo.flags = 0;
        vkCreateSemaphore(m_info->device, &semaphoreInfo, nullptr, &m_info->graphicsSemaphore[i]);
    }
    m_info->presentSemaphore.resize(count);
    for(uint32_t i=0; i<count; ++i){
        VkSemaphoreCreateInfo semaphoreInfo;
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.pNext = nullptr;
        semaphoreInfo.flags = 0;
        vkCreateSemaphore(m_info->device, &semaphoreInfo, nullptr, &m_info->presentSemaphore[i]);
    }
}


void TKBaseInfo::initDescriptorPool() {
    VkDescriptorPoolCreateInfo poolInfo;
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.pNext = nullptr;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 100;
    std::vector<VkDescriptorPoolSize> poolSizeArr(2);
    poolSizeArr[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizeArr[0].descriptorCount = 5;
    poolSizeArr[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizeArr[1].descriptorCount = 5;

    poolInfo.poolSizeCount = poolSizeArr.size();
    poolInfo.pPoolSizes = poolSizeArr.data();
    VkResult ret = vkCreateDescriptorPool(TKBaseInfo::Info()->device, &poolInfo,
										  nullptr, &m_info->descriptorPool);
	if(ret != VK_SUCCESS){
		TKLog("create descriptor pool error %d!\n", ret);
	}
}


void TKBaseInfo::displayInstanceLayers(){
    uint32_t instanceLayerCount = 0;
    vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
    std::vector<VkLayerProperties> layerProperties(instanceLayerCount);
    vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
    for(uint32_t i=0; i<layerProperties.size(); ++i){
        TKLog("\t%s:%s\n", layerProperties[i].layerName, layerProperties[i].description);
    }

    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                           extensionProperties.data());
    TKLog("instance extensions:\n");
    for(uint32_t i=0; i<extensionCount; ++i){
        TKLog("extension: %s, spec ver %d\n", extensionProperties[i].extensionName,
              extensionProperties[i].specVersion);
    }
}

void TKBaseInfo::displayDeviceLayers(){
    uint32_t layerCount;
    vkEnumerateDeviceLayerProperties(m_info->physicalDevice, &layerCount, nullptr);
    std::vector<VkLayerProperties> layerProperties(layerCount);
    vkEnumerateDeviceLayerProperties(m_info->physicalDevice, &layerCount, layerProperties.data());
    TKLog("device layer:\n");
    for(uint32_t i=0; i<layerCount;++i){
        TKLog("\t%s:%s\n", layerProperties[i].layerName, layerProperties[i].description);
    }
    
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(m_info->physicalDevice, nullptr,
                                         &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    vkEnumerateDeviceExtensionProperties(m_info->physicalDevice, nullptr,
                                         &extensionCount, extensionProperties.data());
    TKLog("device extensions:\n");
    for(uint32_t i=0; i<extensionCount; ++i){
        TKLog("extension: %s, spec ver %d\n", extensionProperties[i].extensionName,
              extensionProperties[i].specVersion);
    }
}


uint32_t TKBaseInfo::getMemoryTypeIndex(VkMemoryRequirements memoryReq){
   uint32_t memoryTypeIdx = UINT32_MAX;
    VkMemoryPropertyFlags requiredMemoryFlag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    for (uint32_t i=0; i<TKBaseInfo::Info()->physicalMemoryInfo.memoryTypeCount; ++i){
        VkPhysicalDeviceMemoryProperties memoryInfo = TKBaseInfo::Info()->physicalMemoryInfo;
        VkMemoryPropertyFlags tmpPropertyFlag = memoryInfo.memoryTypes[i].propertyFlags;
        if(memoryReq.memoryTypeBits & (1 << i)){
            if(requiredMemoryFlag == (tmpPropertyFlag & requiredMemoryFlag)){
                memoryTypeIdx = i;
                break;
            }
        }
    }
    return memoryTypeIdx;
}
