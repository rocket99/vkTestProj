//
//TKPipelineLayout.h
//2018-12-26

#ifndef __TK_PipelineLayout_h__
#define __TK_PipelineLayout_h__

#include "TKBaseInfo.h"
#include "TKUniform.h"

class TKPipelineLayout
{
public:
    TKPipelineLayout();
    ~TKPipelineLayout();
    static TKPipelineLayout *createPipelineLayout(const std::string &name);
    bool initPipelineLayout(const std::string &name);

    static TKPipelineLayout *createLayoutFromJson(const Json::Value &value);
    bool initWithJsonValue(const Json::Value &value);
    
    void updateUniformBuffer(const VkDescriptorBufferInfo *uniformBuf,
                             uint32_t dstBind, uint32_t swapIdx);
    void clearAllWriteDescSets();
    
    void bindToCommandBuffer(const VkCommandBuffer &cmdBuf, uint32_t swapchainIdx,
                             const std::vector<VkWriteDescriptorSet> &writeDescSets,
                             const std::vector<VkCopyDescriptorSet>  &copyDescSets);
    
    VkPipelineLayout pipelineLayout() const;
    VkDescriptorSet descriptorSet(uint32_t idx);
private:
    std::string m_name;
    VkPipelineLayout m_layout;
    std::vector<VkDescriptorSetLayout> m_descSetLayouts;
    std::vector<VkDescriptorSet> m_descSets;
    std::vector<VkWriteDescriptorSet> m_writeDescSets;
    bool initBasicPipelineLayout();
    bool initLightPipelineLayout();
};

#endif


