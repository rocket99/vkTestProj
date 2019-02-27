//
//TKPipeline.h
//2018-11-14

#ifndef __TKPipeline_h__
#define __TKPipeline_h__

#include "../TKPrefix.h"
#include "TKPipelineLayout.h"
#include <jsoncpp/json/json.h>

class TKPipeline {
public:
    TKPipeline();
    ~TKPipeline();
	
    static TKPipeline *createGraphicPipeline(const char *vertFile, const char *fragFile,
                                             const char *geomFile = nullptr,
                                             const char *tessCtlFile = nullptr,
                                             const char *tessEvaFile = nullptr);
    bool initGraphicPipeline(const char *vertFile, const char *fragFile,
                             const char *geomFile, const char *tessCtlFile,
                             const char *tessEvaFile);

    bool initPipeline(const std::string &name);
    VkPipeline pipeline() const;

    void initVertexInputState(const std::vector<VkVertexInputBindingDescription> &bindDesc,
                              const std::vector<VkVertexInputAttributeDescription> &attribDesc);
    
    void initAssembleState(VkPrimitiveTopology toplogy, VkBool32 retartEnable=false);
    void initViewportState(const std::vector<VkViewport> &vp, const std::vector<VkRect2D> &sc);
    void initRasterState(VkBool32 depthClamp, VkPolygonMode polygonMode, VkCullModeFlags cullMode,
                         VkFrontFace frontFace, VkBool32 depthBiasEnable, float depthBiasConstFactor,
                         float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth);
    void initTessllationState(uint32_t patchPoints);
    void initColorBlendState(VkBool32 logicOpEnable, VkLogicOp logicOp, float* blendConst,
                             const std::vector<VkPipelineColorBlendAttachmentState> &pAttachments);
    void initDepthStencilState(VkBool32 depthTest, VkBool32 depthWrite, VkCompareOp compareOp,
                               VkBool32 depthBoundTest, VkBool32 stencilTest, float minDepthBounds,
                               float maxDepthBounds, VkStencilOpState front, VkStencilOpState back);
    void initMultipleSampleState(VkSampleCountFlagBits rasterSamples, VkBool32 sampleShade,
                                 float minSampleShading, VkSampleMask *mask,
                                 VkBool32 alphaToCoverage, VkBool32 alphaToOne);

    void initPipelineLayout(const std::string &name);

    VkPipelineLayout pipelineLayout() const;
    VkDescriptorSet descriptorSet(uint32_t idx) const;
    const TKPipelineLayout *getUsingTkPipelineLayout();
    std::string pipelineName() const;
    
private:
    std::string m_name;
    VkPipeline m_pipeline;
    VkShaderModule m_vertModule     = VK_NULL_HANDLE;
    VkShaderModule m_fragModule     = VK_NULL_HANDLE;
    VkShaderModule m_geomModule     = VK_NULL_HANDLE;
    VkShaderModule m_tessCtlModule  = VK_NULL_HANDLE;
    VkShaderModule m_tessEvlModule  = VK_NULL_HANDLE;
    std::map<std::string, std::string> m_moduleMap;

    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
    VkPipelineVertexInputStateCreateInfo    m_vertInputState;
    VkPipelineInputAssemblyStateCreateInfo  m_assembleState;
    VkPipelineTessellationStateCreateInfo   m_tesslState;
    VkPipelineViewportStateCreateInfo       m_viewportState;
    VkPipelineRasterizationStateCreateInfo  m_rasterState;
    VkPipelineMultisampleStateCreateInfo    m_mulSampleState;
    VkPipelineDepthStencilStateCreateInfo   m_depthStencil;
    VkPipelineColorBlendStateCreateInfo     m_colorBlendState;
    VkPipelineDynamicStateCreateInfo        m_dynamicState;

    bool initShaderStageCreateInfo();
    void initDynamicState();

    TKPipelineLayout *m_pipelineLayout = nullptr;     
    
    std::vector<VkVertexInputBindingDescription>     m_inputBindDesc;
    std::vector<VkVertexInputAttributeDescription>   m_inputAttribDesc;
    std::vector<VkPipelineColorBlendAttachmentState> m_blendAttachArr;
    std::vector<VkDynamicState>                      m_dynamicStates;
    std::vector<VkViewport>                          m_viewports;
    std::vector<VkRect2D>                            m_scissorRects;

	void debugVertexInputState();
	void debugInputAssemblyState();
	void debugTessellationState();
	void debugViewportState();
	void debugRasterizationState();
	void debugMultiSampleState();
	void debugDepthStencilState();
	void debugColorBlendState();
};

#endif



