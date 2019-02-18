//
//TKUniform.cpp
//2018-12-30

#include "TKUniform.h"

TKUniform::TKUniform(){
    m_data = nullptr;
    m_size = 0;
    m_buffer = VK_NULL_HANDLE;
    m_memory = VK_NULL_HANDLE;
}

TKUniform::~TKUniform(){
    if(m_buffer != VK_NULL_HANDLE){
        vkDestroyBuffer(TKBaseInfo::Info()->device, m_buffer, nullptr);
    }
    
    if(m_memory != VK_NULL_HANDLE){
        vkFreeMemory(TKBaseInfo::Info()->device, m_memory, nullptr);
    }
    
    if(m_data != nullptr){
        free(m_data);
    }
}

TKUniform * TKUniform::createWithSize(uint32_t size){
    TKUniform *uniform = new TKUniform;
    if(uniform->initWithSize(size) == false){
        delete uniform;
        uniform = nullptr;
    }
    return uniform;
}

bool TKUniform::initWithSize(uint32_t size){
    m_size = size;
    m_data = (float*)malloc(sizeof(float)*size);
    VkBufferCreateInfo bufInfo;
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext = nullptr;
    bufInfo.flags = 0;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufInfo.size  = sizeof(float)*m_size;
    bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    std::vector<uint32_t> queueFamilies = {
        TKBaseInfo::Info()->graphicsQueueFamily
    };
    bufInfo.queueFamilyIndexCount = queueFamilies.size();
    bufInfo.pQueueFamilyIndices   = queueFamilies.data();
    VkResult ret = vkCreateBuffer(TKBaseInfo::Info()->device, &bufInfo, nullptr, &m_buffer);
    if(ret != VK_SUCCESS){
        TKLog("uniform %p create buffer failed! Error:0x%X\n", this, ret);
        return false;
    }
    VkMemoryRequirements memReqInfo;
    vkGetBufferMemoryRequirements(TKBaseInfo::Info()->device, m_buffer, &memReqInfo);
    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = memReqInfo.size;
    allocInfo.memoryTypeIndex = TKBaseInfo::getMemoryTypeIndex(memReqInfo);
    ret = vkAllocateMemory(TKBaseInfo::Info()->device, &allocInfo, nullptr, &m_memory);
    if(ret != VK_SUCCESS){
        TKLog("uniform alloc memory failed!\n");
        return false;
    }
    ret = vkBindBufferMemory(TKBaseInfo::Info()->device, m_buffer, m_memory, 0);
    if(ret != VK_SUCCESS){
        TKLog("bind buffer memory!\n");
        return false;
    }
    return true;
}

void TKUniform::updateData(float *src, uint32_t size, uint32_t offset){
    if(src == nullptr || offset >= sizeof(float)*m_size){
        return;
    }
    memcpy(&m_data[offset], src, size);
}


VkWriteDescriptorSet TKUniform::writeDescSet(const VkDescriptorSet &descSet,
                                             uint32_t dstBind, uint32_t dstElement) {
    VkDevice device = TKBaseInfo::Info()->device;
    void *pData = nullptr;
    VkDeviceSize commitSize;
    vkGetDeviceMemoryCommitment(device, m_memory, &commitSize);
    vkMapMemory(device, m_memory, 0, commitSize, 0, &pData);
    memcpy(pData, m_data, sizeof(float)*m_size);
    vkUnmapMemory(device, m_memory);

    m_descBindInfo.buffer = m_buffer;
    m_descBindInfo.offset = 0;
    m_descBindInfo.range  = sizeof(float)*m_size;

    m_wDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    m_wDescSet.pNext = nullptr;
    m_wDescSet.descriptorCount = 1;
    m_wDescSet.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    m_wDescSet.dstSet          = descSet;
    m_wDescSet.dstBinding      = dstBind;
    m_wDescSet.dstArrayElement = dstElement;
    m_wDescSet.pBufferInfo     = &m_descBindInfo;
    m_wDescSet.pImageInfo      = nullptr;
    m_wDescSet.pTexelBufferView = nullptr;

    return m_wDescSet;
}
