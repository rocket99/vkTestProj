//
//TKPipelineLayout.cpp
//2018-12-26

#include "TKPipelineLayout.h"
#include "TKVkUtility.h"
#include "TKBaseInfo.h"

TKPipelineLayout::TKPipelineLayout(){}
TKPipelineLayout::~TKPipelineLayout(){
    VkDevice device = VK_INFO->device;
    for(uint32_t i=0; i<m_descSets.size(); ++i){
        vkFreeDescriptorSets(device, VK_INFO->descriptorPool, 1, &m_descSets[i]);
    }
    for(uint32_t i=0; i<m_descSetLayouts.size(); ++i){
        vkDestroyDescriptorSetLayout(device, m_descSetLayouts[i], nullptr);
    }
    if(m_layout != VK_NULL_HANDLE){
        vkDestroyPipelineLayout(device, m_layout, nullptr);
    }
}

VkPipelineLayout TKPipelineLayout::pipelineLayout() const {
    return m_layout;
}

VkDescriptorSet TKPipelineLayout::descriptorSet(uint32_t idx){
    uint32_t descCount = VK_INFO->swapchainImageViews.size();
    return m_descSets[idx % descCount];
}

void TKPipelineLayout::clearAllWriteDescSets(){
    m_writeDescSets.empty();
}

void TKPipelineLayout::bindToCommandBuffer(const VkCommandBuffer &cmdBuf, uint32_t swapchainIdx,
                                           const std::vector<VkWriteDescriptorSet> &writeDescSets,
                                           const std::vector<VkCopyDescriptorSet>  &copyDescSets){
    vkUpdateDescriptorSets(VK_INFO->device, writeDescSets.size(),
                           writeDescSets.data(), copyDescSets.size(), copyDescSets.data());
    vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_layout,
                            0, 1, &m_descSets[swapchainIdx], 0, nullptr);
}


TKPipelineLayout *TKPipelineLayout::createLayoutFromJson(const Json::Value &value){
    TKPipelineLayout *layout = new TKPipelineLayout;
    if(layout->initWithJsonValue(value) == false){
        delete layout;
        layout = nullptr;
    }
    return layout;
}

bool TKPipelineLayout::initWithJsonValue(const Json::Value &value){
    if(value.isArray() == false){
        return false;
    }	
    uint32_t size = value.size();
    VkDevice device = VK_INFO->device;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for(uint32_t i=0; i<size; ++i){
        VkDescriptorSetLayoutBinding layoutBind;
        layoutBind.binding         = value[i]["binding"].asUInt();
        layoutBind.descriptorCount = value[i]["descriptorCount"].asUInt();
        layoutBind.descriptorType  = TKVkUtility::VkDecriptorTypeFrom(value[i]["desciptorType"].asString());
        layoutBind.stageFlags      = TKVkUtility::VkShaderStageFlagBitsFrom(value[i]["stageFlags"].asString());
		layoutBind.pImmutableSamplers = nullptr;
        bindings.push_back(layoutBind);
    }
    TKLog("binding count %zu\n", bindings.size());
    uint32_t swapCount = VK_INFO->swapchainImageViews.size();
    m_descSets.resize(swapCount);
    m_descSetLayouts.resize(swapCount);
    for(uint32_t i=0; i<swapCount; ++i){
        std::vector<VkDescriptorSetLayoutCreateInfo> setLayoutCreateInfo(1);
        setLayoutCreateInfo[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        setLayoutCreateInfo[0].pNext = nullptr;
        setLayoutCreateInfo[0].flags = 0;
        setLayoutCreateInfo[0].bindingCount = bindings.size();
        setLayoutCreateInfo[0].pBindings    = bindings.data();
        vkCreateDescriptorSetLayout(device, &setLayoutCreateInfo[0],
                                    nullptr, &m_descSetLayouts[i]);
    }
    
    VkPipelineLayoutCreateInfo layoutInfo;
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = nullptr;
    layoutInfo.flags = 0;
    layoutInfo.setLayoutCount = m_descSetLayouts.size();
    layoutInfo.pSetLayouts    = m_descSetLayouts.data();
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges    = nullptr;
    if(VK_SUCCESS != vkCreatePipelineLayout(device, &layoutInfo, nullptr, &m_layout)){
        TKLog("init pipeline layout failed!\n");
        return false;
    }
	TKLog("init pipeline layout success!\n");
    for(uint32_t i=0; i<swapCount; ++i){
        VkDescriptorSetAllocateInfo descSetAllocInfo;
        descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descSetAllocInfo.pNext = nullptr;
        descSetAllocInfo.descriptorPool = VK_INFO->descriptorPool;
        descSetAllocInfo.descriptorSetCount = 1;
        descSetAllocInfo.pSetLayouts = &m_descSetLayouts[i];
		TKLog("-------%d------\n", i);
        vkAllocateDescriptorSets(device, &descSetAllocInfo, &m_descSets[i]);
		TKLog("alloc descriptor sets\n");
    }
    return true;
}
