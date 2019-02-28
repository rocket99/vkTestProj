//
//TKPipelineLayout.cpp
//2018-12-26

#include "TKPipelineLayout.h"
#include "TKVkUtility.h"
#include "TKBaseInfo.h"

TKPipelineLayout::TKPipelineLayout(){}
TKPipelineLayout::~TKPipelineLayout(){
    VkDevice device = TKBaseInfo::Info()->device;
    for(uint32_t i=0; i<m_descSets.size(); ++i){
        vkFreeDescriptorSets(device, TKBaseInfo::Info()->descriptorPool, 1, &m_descSets[i]);
    }
    for(uint32_t i=0; i<m_descSetLayouts.size(); ++i){
        vkDestroyDescriptorSetLayout(device, m_descSetLayouts[i], nullptr);
    }
    if(m_layout != VK_NULL_HANDLE){
        vkDestroyPipelineLayout(device, m_layout, nullptr);
    }
}
/*
TKPipelineLayout *TKPipelineLayout::createPipelineLayout(const std::string &name){
    TKPipelineLayout *layout = new TKPipelineLayout;
    if(layout->initPipelineLayout(name) == false){
        delete layout;
        layout = nullptr;
    }
    return layout;
}

bool TKPipelineLayout::initPipelineLayout(const std::string &name){
    if(name.compare("basic") == 0){
        return this->initBasicPipelineLayout();
    }
    if(name.compare("light") == 0){
        return this->initLightPipelineLayout();
    }
    m_layout = VK_NULL_HANDLE;
    return true;
}

bool TKPipelineLayout::initBasicPipelineLayout(){
    m_name = "basic";
    VkDevice device = TKBaseInfo::Info()->device;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    VkDescriptorSetLayoutBinding bind0;
    bind0.binding = 0;
    bind0.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bind0.descriptorCount = 1;
    bind0.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
    bind0.pImmutableSamplers = nullptr;
    bindings.push_back(bind0);

    VkDescriptorSetLayoutBinding bind1;
    bind1.binding = 1;
    bind1.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bind1.descriptorCount = 1;
    bind1.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
    bind1.pImmutableSamplers = nullptr;
    bindings.push_back(bind1);

    uint32_t swapCount = TKBaseInfo::Info()->swapchainImageViews.size();
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
    vkCreatePipelineLayout(device, &layoutInfo, nullptr, &m_layout);

    for(uint32_t i=0; i<swapCount; ++i){
        VkDescriptorSetAllocateInfo descSetAllocInfo;
        descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descSetAllocInfo.pNext = nullptr;
        descSetAllocInfo.descriptorPool = TKBaseInfo::Info()->descriptorPool;
        descSetAllocInfo.descriptorSetCount = 1;
        descSetAllocInfo.pSetLayouts = &m_descSetLayouts[i];
        vkAllocateDescriptorSets(device, &descSetAllocInfo, &m_descSets[i]);
    }
    
    TKLog("init basic pipeline layout!\n");
    return true;
}

bool TKPipelineLayout::initLightPipelineLayout(){
    m_name = "light";
    VkDevice device = TKBaseInfo::Info()->device;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    VkDescriptorSetLayoutBinding cameraBind;
    cameraBind.binding = 0;
    cameraBind.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    cameraBind.descriptorCount = 1;
    cameraBind.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
    cameraBind.pImmutableSamplers = nullptr;
    bindings.push_back(cameraBind);

    VkDescriptorSetLayoutBinding transformBind;
    transformBind.binding = 1;
    transformBind.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    transformBind.descriptorCount = 1;
    transformBind.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
    transformBind.pImmutableSamplers = nullptr;
    bindings.push_back(transformBind);

    VkDescriptorSetLayoutBinding materialBind;
    materialBind.binding = 2;
    materialBind.descriptorCount = 2;
    materialBind.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    materialBind.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    materialBind.pImmutableSamplers = nullptr;
    bindings.push_back(materialBind);

    VkDescriptorSetLayoutBinding lightBind;
    lightBind.binding = 3;
    lightBind.descriptorCount = 1;
    lightBind.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lightBind.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    lightBind.pImmutableSamplers = nullptr;
    bindings.push_back(lightBind);

    VkDescriptorSetLayoutBinding eyeBind;
    eyeBind.binding = 4;
    eyeBind.descriptorCount = 1;
    eyeBind.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    eyeBind.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    eyeBind.pImmutableSamplers = nullptr;
    bindings.push_back(eyeBind);

    VkDescriptorSetLayoutBinding transBind;
    transBind.binding = 5;
    transBind.descriptorCount = 1;
    transBind.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    transBind.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    transBind.pImmutableSamplers = nullptr;
    bindings.push_back(transBind);
    
    uint32_t swapCount = TKBaseInfo::Info()->swapchainImageViews.size();
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
    vkCreatePipelineLayout(device, &layoutInfo, nullptr, &m_layout);

    for(uint32_t i=0; i<swapCount; ++i){
        VkDescriptorSetAllocateInfo descSetAllocInfo;
        descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descSetAllocInfo.pNext = nullptr;
        descSetAllocInfo.descriptorPool = TKBaseInfo::Info()->descriptorPool;
        descSetAllocInfo.descriptorSetCount = 1;
        descSetAllocInfo.pSetLayouts = &m_descSetLayouts[i];
        vkAllocateDescriptorSets(device, &descSetAllocInfo, &m_descSets[i]);
    }    
    TKLog("init light pipeline layout!\n");
    return true;
}
*/
VkPipelineLayout TKPipelineLayout::pipelineLayout() const {
    return m_layout;
}

VkDescriptorSet TKPipelineLayout::descriptorSet(uint32_t idx){
    uint32_t descCount = TKBaseInfo::Info()->swapchainImageViews.size();
    return m_descSets[idx % descCount];
}

void TKPipelineLayout::clearAllWriteDescSets(){
    m_writeDescSets.empty();
}

void TKPipelineLayout::bindToCommandBuffer(const VkCommandBuffer &cmdBuf, uint32_t swapchainIdx,
                                           const std::vector<VkWriteDescriptorSet> &writeDescSets,
                                           const std::vector<VkCopyDescriptorSet>  &copyDescSets){
    vkUpdateDescriptorSets(TKBaseInfo::Info()->device, writeDescSets.size(),
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
    VkDevice device = TKBaseInfo::Info()->device;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
   
    for(uint32_t i=0; i<size; ++i){
        VkDescriptorSetLayoutBinding layoutBind;
        layoutBind.binding = value[i]["binding"].asUInt();
        layoutBind.descriptorCount = value[i]["descriptorCount"].asUInt();
        layoutBind.descriptorType = TKVkUtility::VkDecriptorTypeFrom(value[i]["descriptorType"].asString());
        layoutBind.stageFlags = TKVkUtility::VkShaderStageFlagBitsFrom(value[i]["stageFlags"].asString());
        layoutBind.pImmutableSamplers = nullptr;
        bindings.push_back(layoutBind);
    }
    
    uint32_t swapCount = TKBaseInfo::Info()->swapchainImageViews.size();
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
        descSetAllocInfo.descriptorPool = TKBaseInfo::Info()->descriptorPool;
        descSetAllocInfo.descriptorSetCount = 1;
        descSetAllocInfo.pSetLayouts = &m_descSetLayouts[i];
        vkAllocateDescriptorSets(device, &descSetAllocInfo, &m_descSets[i]);
    }
    return true;
}
