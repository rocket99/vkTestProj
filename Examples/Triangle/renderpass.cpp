//
//
//
#include "renderpass.h"
#include <vulkan/vulkan.h>
#include "TKEngine.h"

RenderPass::RenderPass(){}

RenderPass::~RenderPass(){}

RenderPass *RenderPass::create(){
	RenderPass *renderpass = new RenderPass;
	if(renderpass->init()==false){
		delete renderpass;
		renderpass = nullptr;
	}
	return renderpass;
}

bool RenderPass::init(){
	VkAttachmentReference colorAttachRef;
	colorAttachRef.attachment = 0;
	colorAttachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachRef;
	depthAttachRef.attachment = 1;
	depthAttachRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	
	VkSubpassDescription subpassDesc;
	subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDesc.inputAttachmentCount = 0;
	subpassDesc.pInputAttachments = nullptr;
	subpassDesc.colorAttachmentCount = 1;
	subpassDesc.pColorAttachments = &colorAttachRef;
	subpassDesc.pDepthStencilAttachment = &depthAttachRef;
	subpassDesc.flags = 0;
	subpassDesc.preserveAttachmentCount = 0;
	subpassDesc.pPreserveAttachments = nullptr;
	subpassDesc.pResolveAttachments = nullptr;

	VkAttachmentDescription attachDesc[2];
	attachDesc[0].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
	attachDesc[0].format = VK_FORMAT_B8G8R8A8_UNORM;
	attachDesc[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachDesc[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachDesc[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachDesc[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachDesc[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachDesc[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachDesc[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	attachDesc[1].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
	attachDesc[1].format = VK_FORMAT_D32_SFLOAT;
	attachDesc[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachDesc[1].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachDesc[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachDesc[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachDesc[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachDesc[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachDesc[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkRenderPassCreateInfo info;
	info.attachmentCount = 2;
	info.pAttachments = attachDesc;
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.subpassCount = 1;
	info.pSubpasses = &subpassDesc;
	info.dependencyCount = 0;
	info.pDependencies = nullptr;

	VkResult ret = vkCreateRenderPass(VK_INFO->device, &info, nullptr, &m_renderpass);
	if(ret != VK_SUCCESS){
		TKLog("create render pass failed! err = %d", ret);
		return false;
	}
	TKLog("create render pass success!\n");
	return true;
}

VkRenderPass RenderPass::renderPass(){
	return m_renderpass;
}
