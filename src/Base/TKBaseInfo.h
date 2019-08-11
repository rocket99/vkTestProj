//
//TKStructDefine.h
//2018-11-23

#ifndef __TK_BaseInfo_h__
#define __TK_BaseInfo_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


#include "../TKPrefix.h"

#include "TKImageView.h"
#include "TKRenderPass.h"



class TKBaseInfo {
public:
    TKBaseInfo();
    ~TKBaseInfo();
    static TKBaseInfo *share();
    static void purge();
    static BaseVkInfo *Info();
    BaseVkInfo *baseInfo() { return m_info; }
    void enumeratePhysicalDevices();
    void initDevice();
    void initSwapchain();
    void initFramebuffers();
    void initCommandPool();
    void initRenderPass(const char *renderpassJsonFile);
    void initFencesAndSemaphores();
    void initDescriptorPool();
    
    void setGraphicsQueueIndex();
    void setPresentQueueIndex();
    static uint32_t getMemoryTypeIndex(VkMemoryRequirements memoryReq, VkMemoryPropertyFlags flag);
	
private:
    TKRenderPass *m_renderPass;
    TKImageView *m_depthImageView;
    BaseVkInfo *m_info;
    void displayInstanceLayers();
    void displayDeviceLayers();
};

#define VK_INFO  TKBaseInfo::share()->baseInfo()

#endif

