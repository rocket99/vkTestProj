//
//TKUniform.h
//2018-12-30

#ifndef __TKUniform_h__
#define __TKUniform_h__

#include "../TKPrefix.h"
#include "TKBaseInfo.h"

class TKUniform {
public:
    TKUniform();
    ~TKUniform();
    static TKUniform *createWithSize(uint32_t size);
    bool initWithSize(uint32_t size);
    void updateData(float *src, uint32_t size, uint32_t offset);
    VkWriteDescriptorSet writeDescSet(const VkDescriptorSet &descSet,
                                      uint32_t dstBind, uint32_t dstElement=0);
private:
    float *m_data;
    uint32_t m_size;
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;
    VkDescriptorBufferInfo m_descBindInfo;
    VkWriteDescriptorSet m_wDescSet;
        
};


#endif
