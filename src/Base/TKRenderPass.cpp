//
//TKRenderPass.cpp
//2018-11-17

#include "TKRenderPass.h"
#include "TKBaseInfo.h"

TKRenderPass::TKRenderPass(){
    m_renderPass = VK_NULL_HANDLE;
}

TKRenderPass::~TKRenderPass(){
    if(m_renderPass != VK_NULL_HANDLE){
        vkDestroyRenderPass(TKBaseInfo::Info()->device, m_renderPass, nullptr);
    }
}

TKRenderPass *TKRenderPass::createRenderPass(){
    TKRenderPass *renderPass = new TKRenderPass();
    if(false == renderPass->initRenderPass()){
        delete renderPass;
        renderPass = nullptr;
    }
    return renderPass;
}

bool TKRenderPass::initRenderPass(){
    m_colorAttachmentCount = 1;
    std::vector<VkAttachmentReference> inputAttachRef(1);
    inputAttachRef[0].attachment = 0;
    inputAttachRef[0].layout     = VK_IMAGE_LAYOUT_GENERAL;

    std::vector<VkAttachmentReference> colorAttachRef(m_colorAttachmentCount);
    for(int i=0; i<colorAttachRef.size(); ++i){
        colorAttachRef[i].attachment = i;
        colorAttachRef[i].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    VkAttachmentReference depthAttachRef;
    depthAttachRef.attachment = m_colorAttachmentCount;
    depthAttachRef.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::vector<VkSubpassDescription> subPassDesc(1);
    subPassDesc[0].flags                   = 0;
    subPassDesc[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPassDesc[0].inputAttachmentCount    = 0;//inputAttachRef.size();
    subPassDesc[0].pInputAttachments       = nullptr;//inputAttachRef.data();
    subPassDesc[0].colorAttachmentCount    = colorAttachRef.size();
    subPassDesc[0].pColorAttachments       = colorAttachRef.data();
    subPassDesc[0].pDepthStencilAttachment = &depthAttachRef;
    subPassDesc[0].pResolveAttachments     = nullptr;
    subPassDesc[0].preserveAttachmentCount = 0;
    subPassDesc[0].pPreserveAttachments    = nullptr;
    /*
    subPassDesc[1].flags                   = 0;
    subPassDesc[1].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPassDesc[1].inputAttachmentCount    = 0;
    subPassDesc[1].pInputAttachments       = nullptr;
    subPassDesc[1].colorAttachmentCount    = 0;
    subPassDesc[1].pColorAttachments       = nullptr;
    subPassDesc[1].pDepthStencilAttachment = &depthAttachRef;
    subPassDesc[1].pResolveAttachments     = nullptr;
    subPassDesc[1].preserveAttachmentCount = 0;
    subPassDesc[1].pPreserveAttachments    = nullptr;
    */
    //TKLog("render pass format 0x%x\n", TKBaseInfo::Info()->surfaceFormats[0].format);
    
    std::vector<VkAttachmentDescription> attachDesc(m_colorAttachmentCount+1);
    for(int i=0; i<attachDesc.size()-1; ++i){
        attachDesc[i].flags             = 0;
        attachDesc[i].format            = TKBaseInfo::Info()->surfaceFormats[0].format;
        attachDesc[i].loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachDesc[i].storeOp           = VK_ATTACHMENT_STORE_OP_STORE;
        attachDesc[i].stencilLoadOp     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachDesc[i].stencilStoreOp    = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachDesc[i].initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
        attachDesc[i].finalLayout       = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 
        attachDesc[i].samples           = VK_SAMPLE_COUNT_1_BIT; 
    }
    attachDesc[m_colorAttachmentCount].flags           = 0;
    attachDesc[m_colorAttachmentCount].format          = VK_FORMAT_D32_SFLOAT;
    attachDesc[m_colorAttachmentCount].loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachDesc[m_colorAttachmentCount].storeOp         = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachDesc[m_colorAttachmentCount].stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachDesc[m_colorAttachmentCount].stencilStoreOp  = VK_ATTACHMENT_STORE_OP_STORE;
    attachDesc[m_colorAttachmentCount].initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    attachDesc[m_colorAttachmentCount].finalLayout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; 
    attachDesc[m_colorAttachmentCount].samples         = VK_SAMPLE_COUNT_1_BIT;   

    VkSubpassDependency dependency;
    dependency.dependencyFlags = 0;
    dependency.srcSubpass = 0;
    dependency.dstSubpass = 1;
    dependency.srcStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    dependency.srcAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    dependency.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        
    VkRenderPassCreateInfo info;
    info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext           = nullptr;
    info.flags           = 0;
    info.attachmentCount = attachDesc.size();
    info.pAttachments    = attachDesc.data();
    
    info.subpassCount    = subPassDesc.size();
    info.pSubpasses      = subPassDesc.data();

    info.dependencyCount = 0;
    info.pDependencies   = nullptr;//&dependency;
    int ret = vkCreateRenderPass(TKBaseInfo::Info()->device, &info, nullptr, &m_renderPass);
    if(ret != VK_SUCCESS){
        return false;
    }
    TKLog("init render pass success!\n");
    return true;
}

VkRenderPass TKRenderPass::renderPass() const {
    return m_renderPass;
}

uint32_t TKRenderPass::ColorAttachCount() const {
    return m_colorAttachmentCount;
}



