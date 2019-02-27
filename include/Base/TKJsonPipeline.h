//
//TKJsonPipeline.h
//2019-2-22

#ifndef __TKJsonPipeline_h__
#define __TKJsonPipeline_h__

#include "../TKPrefix.h"
#include "TKPipelineLayout.h"
#include "TKVkUtility.h"
#include "TKShaderModule.h"

class TKJsonPipeline {
public:
	TKJsonPipeline();
	~TKJsonPipeline();
	static TKJsonPipeline *createFromJson(const char *file);
	bool initWithJsonFile(const char *file);

	VkPipeline pipeline() const;
	VkPipelineLayout pipelineLayout() const;
	VkDescriptorSet descriptorSet(uint32_t idx) const;
	const TKPipelineLayout * getUsingTkPipelineLayout();
	std::string pipelineName() const;
private:
	std::string m_name;
	std::vector< std::string > m_stageName;
    VkPipeline m_pipeline;
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

    TKPipelineLayout *m_pipelineLayout = nullptr;     
    std::vector<VkVertexInputBindingDescription>     m_inputBindDesc;
    std::vector<VkVertexInputAttributeDescription>   m_inputAttribDesc;
    std::vector<VkPipelineColorBlendAttachmentState> m_blendAttachArr;
    std::vector<VkDynamicState>                      m_dynamicStates;
    std::vector<VkViewport>                          m_viewports;
    std::vector<VkRect2D>                            m_scissorRects;

    bool initShaderStagesFromJson(const Json::Value &value);
    bool initVertexInputStateFromJson(const Json::Value &value);
    bool initAssembleStateFromJson(const Json::Value &value);
    bool initTesslateStateFromJson(const Json::Value &value);
    bool initViewportStateFromJson(const Json::Value &value);
    bool initRasterizationFromJson(const Json::Value &value);
    bool initColorBlendStateFromJson(const Json::Value &value);
    bool initDepthStencilStateFromJson(const Json::Value &value);
    bool initMultiSampleStateFromJson(const Json::Value &value);
    bool initDynamicStateFromJson(const Json::Value &value);
	bool initPipelineLayoutFromJson(const Json::Value &value);
	
	bool initPipeline(const std::string &name);

	void debugShaderStageState();
	void debugVertexInputState();
	void debugInputAssemblyState();
	void debugTessellationState();
	void debugViewportState();
	void debugRasterizationState();
	void debugMultiSampleState();
	void debugDepthStencilState();
	void debugColorBlendState();
	void debugDynamicState();
};

#endif
