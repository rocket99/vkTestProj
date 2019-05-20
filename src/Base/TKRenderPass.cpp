//
//TKRenderPass.cpp
//2018-11-17

#include "TKRenderPass.h"
#include "TKBaseInfo.h"
#include "TKVkUtility.h"

TKRenderPass::TKRenderPass(){
    m_renderPass = VK_NULL_HANDLE;
}

TKRenderPass::~TKRenderPass(){
    if(m_renderPass != VK_NULL_HANDLE){
        vkDestroyRenderPass(VK_INFO->device, m_renderPass, nullptr);
    }
}

TKRenderPass *TKRenderPass::createRenderPass(){
    return TKRenderPass::createWithJson("renderpass.json");
}

TKRenderPass *TKRenderPass::createWithJson(const std::string &fileName){
	TKRenderPass *renderPass = new TKRenderPass;
  	if(renderPass->initWithJson(fileName)==false){
		delete renderPass;
		renderPass = nullptr;
	}
	return renderPass;
}

bool TKRenderPass::initWithJson(const std::string &fileName){
	char buf0[512];
	sprintf(buf0, "%s/Json/%s", PROJECT_PATH, fileName.c_str());
	FILE *fp = fopen(buf0, "rt");
	if(fp == nullptr){
		TKLog("Open file %s failed!\n", fileName.c_str());
		assert(fp != nullptr);
		return false;
	}
	char tmp;
	uint64_t size = 1;
	while(fread(&tmp, sizeof(char), 1, fp)==1){
		++ size;
	}
	char buf[size];
	buf[size-1] = '\0';
	fseek(fp, 0, SEEK_SET);
	fread(buf, sizeof(char), size, fp);
	fclose(fp);
	
	std::string content(buf);
	Json::Value root;
	Json::Reader reader;
	reader.parse(content, root);

	VkRenderPassCreateInfo info;
	Json::Value subpassDesValue = root["subpassDesc"];
	std::vector<VkSubpassDescription> subpassDescArr;
	if(false == subpassDesValue.isArray()){
		return false;
	}else{
		for(uint32_t i=0; i<subpassDesValue.size();++i){
			subpassDescArr.push_back(this->_getSubpassDescFromJson(subpassDesValue[i], i));
		}
	}
	info.subpassCount = subpassDescArr.size();
	info.pSubpasses = subpassDescArr.data();
  
	Json::Value attachDescValue = root["attachDesc"];
	std::vector<VkAttachmentDescription> attachmentDescArr;
	if(false == attachDescValue.isArray()){
		return false;
	}else{
		for(uint32_t i=0; i<attachDescValue.size(); ++i){
			attachmentDescArr.push_back(this->_getAttachmentDescription(attachDescValue[i]));
		}
	}
	info.attachmentCount = attachmentDescArr.size();
	info.pAttachments = attachmentDescArr.data();

	Json::Value dependencyValue = root["dependencies"];
	if(false == dependencyValue.isArray()){
		return false;
	}
	std::vector<VkSubpassDependency> subpassDepArr;
	if(dependencyValue.size()==0){
		info.dependencyCount = 0;
		info.pDependencies = nullptr;
	}else{
		for(uint32_t i=0; i<dependencyValue.size(); ++i){
			subpassDepArr.push_back(this->_getDependencyFromJson(dependencyValue[i]));
		}
	}
	info.dependencyCount = subpassDepArr.size();
	info.pDependencies = subpassDepArr.data();
	
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;

	int ret = vkCreateRenderPass(VK_INFO->device, &info, nullptr, &m_renderPass);
	if(ret != VK_SUCCESS){
		return false;
	}
	TKLog("init render pass with json success");
	return true;
}

VkRenderPass TKRenderPass::renderPass() const {
    return m_renderPass;
}

uint32_t TKRenderPass::ColorAttachCount() const {
    return m_colorAttachmentCount;
}

VkSubpassDependency TKRenderPass::_getDependencyFromJson(const Json::Value &value){
	VkSubpassDependency dependency;
	dependency.dependencyFlags = value["dependencyFlags"].asUInt();
	dependency.srcSubpass = value["srcSubpass"].asUInt();
	dependency.dstSubpass = value["dstSubpass"].asUInt();
	dependency.srcStageMask = TKVkUtility::VkPipelineStageFlagBitFrom(value["srcStageMask"].asString());
	dependency.dstStageMask = TKVkUtility::VkPipelineStageFlagBitFrom(value["dstStageMask"].asString());
	if(value["srcAccessMask"].isArray()==true){
		TKLog("access mask: %s\n", value["srcAccessMask"][0].asCString());
		uint32_t accessMask = TKVkUtility::VkAccessFlagBitFrom(value["srcAccessMask"][0].asString());
		for(uint32_t i=1; i<value["srcAccessMask"].size(); ++i){
			TKLog("access mask: %s\n", value["srcAccessMask"][i].asCString());
			accessMask |= TKVkUtility::VkAccessFlagBitFrom(value["srcAccessMask"][i].asString());
		}
		dependency.srcAccessMask = accessMask;
	}

	if(value["dstAccessMask"].isArray()==true){
		TKLog("access mask: %s\n", value["srcAccessMask"][0].asCString());
		uint32_t accessMask = TKVkUtility::VkAccessFlagBitFrom(value["dstAccessMask"][0].asString());;
		for(uint32_t i=1; i<value["dstAccessMask"].size(); ++i){
			TKLog("access mask: %s\n", value["srcAccessMask"][i].asCString());
			accessMask |= TKVkUtility::VkAccessFlagBitFrom(value["dstAccessMask"][i].asString());
		}
		dependency.dstAccessMask = accessMask;
	}
	return dependency;
}

VkAttachmentReference TKRenderPass::_getAttachmentRefFromJson(const Json::Value &value){
	VkAttachmentReference attachRef;
	attachRef.attachment = value["attachment"].asUInt();
	attachRef.layout = TKVkUtility::VkImageLayoutFromString(value["layout"].asString());
	return attachRef;
}

VkSubpassDescription TKRenderPass::_getSubpassDescFromJson(const Json::Value &value, uint32_t idx){
	std::string idxStr = std::to_string(idx);
	VkSubpassDescription subpassDesc;
	subpassDesc.pipelineBindPoint = TKVkUtility::VkPipelineBindPointFromString(value["pipelineBindPoint"].asString());
	std::vector<VkAttachmentReference> inputRefArr;
	if(value["inputAttachments"].isArray()==true){
		for(uint32_t i=0; i<value["inputAttachments"].size(); ++i){
			inputRefArr.push_back(this->_getAttachmentRefFromJson(value["inputAttachments"][i]));
		}
	}
	m_allAttachmentRefInfo["input"+idxStr] = inputRefArr;
	std::vector<VkAttachmentReference> colorRefArr;
	if(value["colorAttachments"].isArray()==true){
		for(uint32_t i=0; i<value["colorAttachments"].size(); ++i){
			inputRefArr.push_back(this->_getAttachmentRefFromJson(value["colorAttachments"][i]));
		}
	}
	m_allAttachmentRefInfo["color"+idxStr] = colorRefArr;
	std::vector<VkAttachmentReference> depthRefArr;
	if(value["depthAttachments"].isArray()==true){
		for(uint32_t i=0; i<value["depthAttachments"].size(); ++i){
			inputRefArr.push_back(this->_getAttachmentRefFromJson(value["depthAttachments"][i]));
		}
	}
	m_allAttachmentRefInfo["depth"+idxStr] = depthRefArr;
	std::vector<VkAttachmentReference> resolveRefArr;
	if(value["resolveAttachments"].isArray()==true){
		for(uint32_t i=0; i<value["resolveAttachments"].size(); ++i){
			inputRefArr.push_back(this->_getAttachmentRefFromJson(value["resolveAttachments"][i]));
		}
	}
	m_allAttachmentRefInfo["resolve"+idxStr] = resolveRefArr;
	
	std::vector<uint32_t> preserveAttachments;
	if(value["preserveAttachments"].isArray()==true){
		for(uint32_t i=0; i<value["preserveAttachments"].size(); ++i){
			preserveAttachments.push_back(value["preserveAttachments"][i].asUInt());
		}
	}
	m_IntAttachInfo["preserve"+idxStr] = preserveAttachments;
	
	subpassDesc.colorAttachmentCount = m_allAttachmentRefInfo["color"+idxStr].size();
	subpassDesc.pColorAttachments = m_allAttachmentRefInfo["color"+idxStr].data();
	subpassDesc.inputAttachmentCount = m_allAttachmentRefInfo["input"+idxStr].size();
	subpassDesc.pInputAttachments = m_allAttachmentRefInfo["input"+idxStr].data();
	subpassDesc.pResolveAttachments = m_allAttachmentRefInfo["resolve"+idxStr].data();
	subpassDesc.pDepthStencilAttachment = m_allAttachmentRefInfo["depth"+idxStr].data();
	subpassDesc.preserveAttachmentCount = m_IntAttachInfo["preserve"+idxStr].size();
	subpassDesc.pPreserveAttachments = m_IntAttachInfo["preserve"+idxStr].data();
	
	return subpassDesc;
}

VkAttachmentDescription TKRenderPass::_getAttachmentDescription(const Json::Value &value){
	VkAttachmentDescription attachDesc;
	attachDesc.format = TKVkUtility::VkFormatFrom(value["format"].asString());
	attachDesc.loadOp = TKVkUtility::VkAttachmentLoadOpFromString(value["loadOp"].asString());
	attachDesc.storeOp = TKVkUtility::VkAttachmentStoreOpFromString(value["storeOp"].asString());
	attachDesc.stencilLoadOp = TKVkUtility::VkAttachmentLoadOpFromString(value["stencilLoadOp"].asString());
	attachDesc.stencilStoreOp = TKVkUtility::VkAttachmentStoreOpFromString(value["stencilStoreOp"].asString());
	attachDesc.initialLayout = TKVkUtility::VkImageLayoutFromString(value["initialLayout"].asString());
	attachDesc.finalLayout = TKVkUtility::VkImageLayoutFromString(value["finalLayout"].asString());
	attachDesc.samples = TKVkUtility::VkSampleCountFlagBitFrom(value["samples"].asString());
	return attachDesc;
}


