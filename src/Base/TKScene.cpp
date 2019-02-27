//
//TKScene.cpp
//2018-11-14

#include "TKScene.h"
#include "TKBaseInfo.h"
#include "TKPipelineManager.h"
#include <unistd.h>

TKScene::TKScene() {
    m_width = SCREEN_WIDTH;
    m_height = SCREEN_HEIGHT;
}

TKScene::~TKScene() {
    TKLog("destructed TKScene\n");
    for(int i=0; i<m_drawNodes.size(); ++i){
        TKBaseNode *node = m_drawNodes[i];
        delete  node;
    }
}

TKScene *TKScene::scene(uint32_t width, uint32_t height) {
    TKScene *s = new TKScene();
    if(false == s->init(width, height)){
        delete s;
        s = nullptr;
    }
    return s;
}

bool TKScene::init(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
    m_currentIdx = 0;
    vkDeviceWaitIdle(TKBaseInfo::Info()->device);
    TKLog("init scene success!\n");
    return true;
}

void TKScene::updateDrawCommand(){
    //TKLog("update command image indice %d\n", m_currentIdx);
    vkResetCommandBuffer(TKBaseInfo::Info()->commandBuffers[m_currentIdx],
                         VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    VkCommandBufferBeginInfo cmdBufBeginInfo;
    cmdBufBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufBeginInfo.pNext = nullptr;
    cmdBufBeginInfo.flags =
        VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT |
        VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    cmdBufBeginInfo.pInheritanceInfo = nullptr;
    VkResult result = vkBeginCommandBuffer(TKBaseInfo::Info()->commandBuffers[m_currentIdx],
                                           &cmdBufBeginInfo);
    if(result != VK_SUCCESS){
        TKLog("begin command buffer error!\n");
    }

    VkImageMemoryBarrier presentToDrawBarrier;
    presentToDrawBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    presentToDrawBarrier.pNext = nullptr;
    presentToDrawBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    presentToDrawBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    presentToDrawBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    presentToDrawBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    if(TKBaseInfo::Info()->graphicsQueueFamily == TKBaseInfo::Info()->presentQueueFamily){
        presentToDrawBarrier.srcQueueFamilyIndex = TKBaseInfo::Info()->graphicsQueueFamily;
        presentToDrawBarrier.dstQueueFamilyIndex = TKBaseInfo::Info()->presentQueueFamily;
    }else{
        presentToDrawBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        presentToDrawBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    }
    presentToDrawBarrier.image = TKBaseInfo::Info()->swapchainImages[m_currentIdx];
    
    VkImageSubresourceRange subResRange;
    subResRange.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
    subResRange.baseMipLevel   = 0;
    subResRange.levelCount      = 1;
    subResRange.baseArrayLayer  = 0;
    subResRange.layerCount      = 1;
    presentToDrawBarrier.subresourceRange = subResRange;
    vkCmdPipelineBarrier(TKBaseInfo::Info()->commandBuffers[m_currentIdx],
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &presentToDrawBarrier);

    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass  = TKBaseInfo::Info()->renderPass;
    beginInfo.framebuffer = TKBaseInfo::Info()->framebuffers[m_currentIdx];
    beginInfo.renderArea  = { {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT} };
    std::vector<VkClearValue> clearValues(2);
	const float values[] = {0.5, 0.5, 0.5, 1.0};
    memcpy(clearValues[0].color.float32, values, sizeof(float)*4);
    clearValues[1].depthStencil.depth = 1.0f;
    clearValues[1].depthStencil.stencil = 0.0f;
    beginInfo.clearValueCount = clearValues.size();
    beginInfo.pClearValues    = clearValues.data();
    vkCmdBeginRenderPass(TKBaseInfo::Info()->commandBuffers[m_currentIdx],
                         &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport = {0, float(m_height), float(m_width), -float(m_height), 0.0, 1.0};
    VkRect2D rect = {{0, 0}, {m_width, m_height}};
    vkCmdSetViewport(TKBaseInfo::Info()->commandBuffers[m_currentIdx], 0, 1, &viewport);
    vkCmdSetScissor(TKBaseInfo::Info()->commandBuffers[m_currentIdx], 0, 1, &rect);
    vkCmdSetLineWidth(TKBaseInfo::Info()->commandBuffers[m_currentIdx], 1.0);
    
    this->drawObjects();
    vkCmdEndRenderPass(TKBaseInfo::Info()->commandBuffers[m_currentIdx]);
    if(TKBaseInfo::Info()->presentQueueFamily != TKBaseInfo::Info()->graphicsQueueFamily){
        VkImageSubresourceRange subResourceRange;
        subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subResourceRange.baseMipLevel = 0;
        subResourceRange.levelCount = 1;
        subResourceRange.baseArrayLayer = 0;
        subResourceRange.layerCount = 1;
        VkImageMemoryBarrier drawToPresentBarrier;
        drawToPresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        drawToPresentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        drawToPresentBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        drawToPresentBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        drawToPresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        drawToPresentBarrier.srcQueueFamilyIndex = TKBaseInfo::Info()->graphicsQueueFamily;
        drawToPresentBarrier.dstQueueFamilyIndex = TKBaseInfo::Info()->presentQueueFamily;
        drawToPresentBarrier.image = TKBaseInfo::Info()->swapchainImages[m_currentIdx];
        drawToPresentBarrier.subresourceRange = subResourceRange;
        vkCmdPipelineBarrier(TKBaseInfo::Info()->commandBuffers[m_currentIdx],
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &drawToPresentBarrier);
    }

    VkResult ret = vkEndCommandBuffer(TKBaseInfo::Info()->commandBuffers[m_currentIdx]);
    if(ret != VK_SUCCESS){
        TKLog("end command buffer error!\n");
    }
}

void TKScene::drawObjects(){
    TKLog("draw object!\n");
}

void TKScene::renderDraw(){
    VkResult result;
    do{
        result = vkAcquireNextImageKHR(TKBaseInfo::Info()->device, TKBaseInfo::Info()->swapchain,
                                       UINT64_MAX, TKBaseInfo::Info()->graphicsSemaphore[m_frameIdx],
                                       VK_NULL_HANDLE, &m_currentIdx);
        // TKLog("result %x\n", result);
    }while(result != VK_SUCCESS);
    //TKLog("frame idx: %d, current idx = %d\n", m_frameIdx, m_currentIdx);

    this->updateDrawCommand();
    //submit
    VkSubmitInfo submitInfo;
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext                = nullptr;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = &TKBaseInfo::Info()->graphicsSemaphore[m_frameIdx];
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    submitInfo.pWaitDstStageMask    = &pipe_stage_flags;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &(TKBaseInfo::Info()->commandBuffers[m_currentIdx]);
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &TKBaseInfo::Info()->presentSemaphore[m_frameIdx];
    vkQueueWaitIdle(TKBaseInfo::Info()->graphicsQueue);
    VkResult ret = vkQueueSubmit(TKBaseInfo::Info()->graphicsQueue, 1, &submitInfo,
                                 TKBaseInfo::Info()->fences[m_currentIdx]);
    if (VK_SUCCESS != ret){
        TKLog("queue submit failed! Error: %d\n", ret);
    }

    vkWaitForFences(TKBaseInfo::Info()->device, 1, &TKBaseInfo::Info()->fences[m_currentIdx], VK_TRUE, 200);
    vkResetFences(TKBaseInfo::Info()->device, 1, &TKBaseInfo::Info()->fences[m_currentIdx]);
 
    //present 
    VkPresentInfoKHR presentInfo;
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext              = nullptr;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = &TKBaseInfo::Info()->presentSemaphore[m_frameIdx];
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = &TKBaseInfo::Info()->swapchain;
    presentInfo.pImageIndices      = &m_currentIdx;
    presentInfo.pResults           = nullptr;
    vkQueueWaitIdle(TKBaseInfo::Info()->presentQueue);
    ret = vkQueuePresentKHR(TKBaseInfo::Info()->presentQueue, &presentInfo);
    switch(ret){
    case VK_SUCCESS:
        m_frameIdx += 1;
        m_frameIdx %= TKBaseInfo::Info()->framebuffers.size();
        break;
    default:
        TKLog("Problem occured during image presentation!\n");
        break;
    } 
}

void TKScene::stopDraw(){
    vkDeviceWaitIdle(TKBaseInfo::Info()->device);   
}



