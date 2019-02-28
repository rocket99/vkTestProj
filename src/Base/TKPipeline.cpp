//
//TKPipeline.cpp
//2018-11-14

#include "TKPipeline.h"
#include "TKBaseInfo.h"
#include "TKShaderModule.h"
#include "TKVkUtility.h"

TKPipeline::TKPipeline(){
    m_pipeline = VK_NULL_HANDLE;
}

TKPipeline::~TKPipeline(){
    TKLog("destruct pipeline object %p!\n", this);
    delete m_pipelineLayout;
    if(m_pipeline != VK_NULL_HANDLE){
        vkDestroyPipeline(TKBaseInfo::Info()->device, m_pipeline, nullptr);
    }
}

TKPipeline *TKPipeline::createGraphicPipeline(const char *vertFile, const char *fragFile,
                                              const char *geomFile, const char *tessCtlFile,
                                              const char *tessEvaFile){
    TKPipeline *pipeline = new TKPipeline();
    if(pipeline->initGraphicPipeline(vertFile, fragFile, geomFile,
                                     tessCtlFile, tessEvaFile) == false){
        delete pipeline;
        pipeline = nullptr;
    }
    return pipeline;
}

bool TKPipeline::initGraphicPipeline(const char *vertFile, const char *fragFile,
                                     const char *geomFile, const char *tessCtlFile,
                                     const char *tessEvaFile){
    TKLog("create graphics pipeline %p\n", this);

    m_moduleMap["vertex"] = vertFile;
    m_moduleMap["fragment"] = fragFile;

    if(geomFile != nullptr){
        m_moduleMap["geometry"] = geomFile;
    }else{
		m_moduleMap["geometry"] = "";
	}
    if(tessCtlFile != nullptr){
        m_moduleMap["tess_ctrl"] = tessCtlFile;
    }else{
		m_moduleMap["tess_ctrl"] = "";
	}
    if(tessEvaFile != nullptr){
        m_moduleMap["tess_evaluation"] = tessEvaFile;
    }else{
		m_moduleMap["tess_evaluation"] = "";
	}
	
    if(false == this->initShaderStageCreateInfo()){
        return false;
    }
    this->initDynamicState();
	
    return true;
}

bool TKPipeline::initPipeline(const std::string &name){
	m_name = name;
    VkGraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext               = nullptr;
    pipelineInfo.flags               = 0;
    pipelineInfo.stageCount          = m_shaderStages.size();
    pipelineInfo.pStages             = m_shaderStages.data();
	//this->debugVertexInputState();
    pipelineInfo.pVertexInputState   = &m_vertInputState;
	//this->debugInputAssemblyState();
    pipelineInfo.pInputAssemblyState = &m_assembleState;
	//this->debugTessellationState();
    pipelineInfo.pTessellationState  = &m_tesslState;
	//this->debugViewportState();
    pipelineInfo.pViewportState      = &m_viewportState;
	//this->debugRasterizationState();
    pipelineInfo.pRasterizationState = &m_rasterState;
	//this->debugMultiSampleState();
    pipelineInfo.pMultisampleState   = &m_mulSampleState;
	//this->debugDepthStencilState();
    pipelineInfo.pDepthStencilState  = &m_depthStencil;
	//this->debugColorBlendState();
    pipelineInfo.pColorBlendState    = &m_colorBlendState;

	pipelineInfo.pDynamicState       = &m_dynamicState;
    pipelineInfo.layout              = m_pipelineLayout->pipelineLayout();
    pipelineInfo.renderPass          = TKBaseInfo::Info()->renderPass;
    pipelineInfo.subpass             = 0;
    pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex   = -1;

	TKLog("init pipeline %s\n", m_name.c_str());
    VkResult ret = vkCreateGraphicsPipelines(TKBaseInfo::Info()->device, VK_NULL_HANDLE,
                                             1, &pipelineInfo, nullptr, &m_pipeline);
    if (ret != VK_SUCCESS) {
        printf("init pipeline failed! ret= %d\n", ret);
        return false;
    }
    TKLog("init pipeline success!\n");    
    return true;
}

bool TKPipeline::initShaderStageCreateInfo() {
	m_vertModule = TKModuleManager::shaderModule(m_moduleMap["vertex"].c_str());
    m_fragModule = TKModuleManager::shaderModule(m_moduleMap["fragment"].c_str());
	if(strlen(m_moduleMap["geometry"].c_str()) > 4){
		m_geomModule = TKModuleManager::shaderModule(m_moduleMap["geometry"].c_str());
	}
	if(strlen(m_moduleMap["tess_ctrl"].c_str()) > 4){
		m_tessCtlModule = TKModuleManager::shaderModule(m_moduleMap["tess_ctrl"].c_str());
	}
	if(strlen(m_moduleMap["tess_evaluation"].c_str()) > 4){
		m_tessEvlModule = TKModuleManager::shaderModule(m_moduleMap["tess_evaluation"].c_str());
	}
    uint32_t moduleCnt = 0;
    if(m_vertModule != VK_NULL_HANDLE){
        ++ moduleCnt;
    }else{
        return false;
    }
    if(m_fragModule != VK_NULL_HANDLE){
        ++ moduleCnt;
    }else{
        return false;
    }
    if(m_geomModule != VK_NULL_HANDLE){
        ++ moduleCnt;
    }
    if(m_tessCtlModule != VK_NULL_HANDLE){
        ++ moduleCnt;
    }
    if(m_tessEvlModule != VK_NULL_HANDLE){
        ++ moduleCnt;
    }
    m_shaderStages.resize(moduleCnt);
    m_shaderStages[0].sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_shaderStages[0].pNext    = nullptr;
    m_shaderStages[0].flags    = 0;
    m_shaderStages[0].stage    = VK_SHADER_STAGE_VERTEX_BIT;
    m_shaderStages[0].module   = m_vertModule;
    m_shaderStages[0].pName    = "main";
    m_shaderStages[0].pSpecializationInfo = nullptr;
    
    m_shaderStages[1].sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_shaderStages[1].pNext    = nullptr;
    m_shaderStages[1].flags    = 0;
    m_shaderStages[1].stage    = VK_SHADER_STAGE_FRAGMENT_BIT;
    m_shaderStages[1].module   = m_fragModule;
    m_shaderStages[1].pName    = "main";
    m_shaderStages[1].pSpecializationInfo = nullptr;

    if(m_geomModule != VK_NULL_HANDLE){
        m_shaderStages[2].sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_shaderStages[2].pNext    = nullptr;
        m_shaderStages[2].flags    = 0;
        m_shaderStages[2].stage    = VK_SHADER_STAGE_GEOMETRY_BIT;
        m_shaderStages[2].module   = m_geomModule;
        m_shaderStages[2].pName    = "main";
        m_shaderStages[2].pSpecializationInfo = nullptr;
    }
    if(m_tessCtlModule != VK_NULL_HANDLE){
        m_shaderStages[3].sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_shaderStages[3].pNext    = nullptr;
        m_shaderStages[3].flags    = 0;
        m_shaderStages[3].stage    = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        m_shaderStages[3].module   = m_tessCtlModule;
        m_shaderStages[3].pName    = "main";
        m_shaderStages[3].pSpecializationInfo = nullptr;
    }
    if(m_tessEvlModule != VK_NULL_HANDLE){
        m_shaderStages[4].sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_shaderStages[4].pNext    = nullptr;
        m_shaderStages[4].flags    = 0;
        m_shaderStages[4].stage    = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        m_shaderStages[4].module   = m_tessEvlModule;
        m_shaderStages[4].pName    = "main";
        m_shaderStages[4].pSpecializationInfo = nullptr;
    }
    return true;
}

void TKPipeline::initVertexInputState(const std::vector<VkVertexInputBindingDescription> &bindDesc,
                                      const std::vector<VkVertexInputAttributeDescription> &attribDesc){
    m_inputBindDesc = bindDesc;
    m_inputAttribDesc = attribDesc;
    m_vertInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    m_vertInputState.pNext = nullptr;
    m_vertInputState.flags = 0;
    m_vertInputState.vertexBindingDescriptionCount   = m_inputBindDesc.size();
    m_vertInputState.pVertexBindingDescriptions      = m_inputBindDesc.data();
    m_vertInputState.vertexAttributeDescriptionCount = m_inputAttribDesc.size();
    m_vertInputState.pVertexAttributeDescriptions    = m_inputAttribDesc.data();
}

void TKPipeline::initAssembleState(const VkPrimitiveTopology toplogy, const VkBool32 restartEnable){
    m_assembleState.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_assembleState.pNext    = nullptr;
    m_assembleState.flags    = 0;
    m_assembleState.topology = toplogy;
    m_assembleState.primitiveRestartEnable = restartEnable;
}

void TKPipeline::initViewportState(const std::vector<VkViewport> &vp, const std::vector<VkRect2D> &sc){
    m_viewports    = vp;
    m_scissorRects = sc;
    m_viewportState.sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    m_viewportState.pNext          = nullptr;
    m_viewportState.flags          = 0;
    m_viewportState.viewportCount  = m_viewports.size();
    m_viewportState.pViewports     = m_viewports.data();
    m_viewportState.scissorCount   = m_scissorRects.size();
    m_viewportState.pScissors      = m_scissorRects.data();
}

void TKPipeline::initTessllationState(uint32_t patchPoints){
    m_tesslState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    m_tesslState.flags = 0;
    m_tesslState.pNext = nullptr;
    m_tesslState.patchControlPoints = patchPoints;
}

void TKPipeline::initRasterState(VkBool32 depthClamp, VkPolygonMode polygonMode, VkCullModeFlags cullMode,
                                 VkFrontFace frontFace, VkBool32 depthBiasEnable, float depthBiasConstFactor,
                                 float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth){
    m_rasterState.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_rasterState.pNext                   = nullptr;
    m_rasterState.flags                   = 0;
    m_rasterState.depthClampEnable        = depthClamp;
    m_rasterState.rasterizerDiscardEnable = VK_FALSE;
    m_rasterState.polygonMode             = polygonMode;
    m_rasterState.cullMode                = cullMode;
    m_rasterState.frontFace               = frontFace;
    m_rasterState.depthBiasEnable         = depthBiasEnable;
    m_rasterState.depthBiasConstantFactor = depthBiasConstFactor;
    m_rasterState.depthBiasClamp          = depthBiasClamp;
    m_rasterState.depthBiasSlopeFactor    = depthBiasSlopeFactor;
    m_rasterState.lineWidth               = lineWidth;
}

void TKPipeline::initMultipleSampleState(VkSampleCountFlagBits rasterSamples, VkBool32 sampleShade,
                                         float minSampleShading, VkSampleMask *mask,
                                         VkBool32 alphaToCoverage, VkBool32 alphaToOne){
    m_mulSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_mulSampleState.pNext = nullptr;
    m_mulSampleState.flags = 0;
    m_mulSampleState.rasterizationSamples  = rasterSamples;
    m_mulSampleState.sampleShadingEnable   = sampleShade;
    m_mulSampleState.minSampleShading      = minSampleShading;
    m_mulSampleState.pSampleMask           = mask;
    m_mulSampleState.alphaToCoverageEnable = alphaToCoverage;
    m_mulSampleState.alphaToOneEnable      = alphaToOne;
}

void TKPipeline::initDepthStencilState(VkBool32 depthTest, VkBool32 depthWrite, VkCompareOp compareOp,
                                       VkBool32 depthBoundTest, VkBool32 stencilTest, float minDepthBounds,
                                       float maxDepthBounds, VkStencilOpState front, VkStencilOpState back){
    m_depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    m_depthStencil.pNext                 = nullptr;
    m_depthStencil.flags                 = 0;
    m_depthStencil.depthTestEnable       = depthTest;
    m_depthStencil.depthWriteEnable      = depthWrite;
    m_depthStencil.depthCompareOp        = compareOp;
    m_depthStencil.depthBoundsTestEnable = depthBoundTest;
    m_depthStencil.front.failOp          = front.failOp;
    m_depthStencil.front.passOp          = front.passOp;
    m_depthStencil.front.depthFailOp     = front.depthFailOp;
    m_depthStencil.front.compareOp       = front.compareOp;
    m_depthStencil.back.failOp           = back.failOp;
    m_depthStencil.back.passOp           = back.passOp;
    m_depthStencil.back.depthFailOp      = back.depthFailOp;
    m_depthStencil.back.compareOp        = back.compareOp;
    m_depthStencil.stencilTestEnable     = stencilTest;
    m_depthStencil.maxDepthBounds        = maxDepthBounds;
    m_depthStencil.minDepthBounds        = minDepthBounds;
}

void TKPipeline::initColorBlendState(VkBool32 logicOpEnable, VkLogicOp logicOp, float* blendConst,
                                     const std::vector<VkPipelineColorBlendAttachmentState> &pAttachments){
    m_blendAttachArr = pAttachments;
    m_colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_colorBlendState.pNext = nullptr;
    m_colorBlendState.flags = 0;
    m_colorBlendState.logicOpEnable      = logicOpEnable;
    m_colorBlendState.logicOp            = logicOp;
    m_colorBlendState.attachmentCount    = m_blendAttachArr.size();
    m_colorBlendState.pAttachments       = m_blendAttachArr.data();
    m_colorBlendState.blendConstants[0]  = blendConst[0];
    m_colorBlendState.blendConstants[1]  = blendConst[1];
    m_colorBlendState.blendConstants[2]  = blendConst[2];
    m_colorBlendState.blendConstants[3]  = blendConst[3];
}

void TKPipeline::initDynamicState(){
    m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_dynamicState.pNext = nullptr;
    m_dynamicState.flags = 0;

    m_dynamicStates.resize(4);
    m_dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH, VK_DYNAMIC_STATE_DEPTH_BIAS};
    m_dynamicState.dynamicStateCount  = m_dynamicStates.size();
    m_dynamicState.pDynamicStates     = m_dynamicStates.data();
}

void TKPipeline::initPipelineLayout(const std::string &name){
	// m_pipelineLayout = TKPipelineLayout::createPipelineLayout(name);
}

VkPipeline TKPipeline::pipeline() const {
    return m_pipeline;
}

VkPipelineLayout TKPipeline::pipelineLayout() const {
    return m_pipelineLayout->pipelineLayout();
}

VkDescriptorSet TKPipeline::descriptorSet(uint32_t idx) const{
    return m_pipelineLayout->descriptorSet(idx);
}

const TKPipelineLayout *TKPipeline::getUsingTkPipelineLayout(){
    return m_pipelineLayout;
}

std::string TKPipeline::pipelineName() const{
    return m_name;
}


void TKPipeline::debugVertexInputState(){
	Json::Value value;
	Json::StyledWriter writer;
	for(uint32_t i=0; i<m_vertInputState.vertexBindingDescriptionCount; ++i){
		value["bindDesc"][i]["binding"] = m_vertInputState.pVertexBindingDescriptions[i].binding;
		value["bindDesc"][i]["stride"] = m_vertInputState.pVertexBindingDescriptions[i].stride;
		value["bindDesc"][i]["inputRate"] = m_vertInputState.pVertexBindingDescriptions[i].inputRate;
	}
	for(uint32_t i=0; i<m_vertInputState.vertexAttributeDescriptionCount; ++i){
		value["attrDesc"][i]["location"] = m_vertInputState.pVertexAttributeDescriptions[i].location;
		value["attrDesc"][i]["binding"] = m_vertInputState.pVertexAttributeDescriptions[i].binding;
		value["attrDesc"][i]["offset"] = m_vertInputState.pVertexAttributeDescriptions[i].offset;
		value["attrDesc"][i]["format"] = m_vertInputState.pVertexAttributeDescriptions[i].format;
	}
	value["type"] = m_vertInputState.sType;
	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}

void TKPipeline::debugInputAssemblyState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["topology"] = m_assembleState.topology;
	value["primitive"] = m_assembleState.primitiveRestartEnable;

	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}

void TKPipeline::debugTessellationState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["patchControlPoints"] = m_tesslState.patchControlPoints;
	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}

void TKPipeline::debugViewportState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["viewportCount"] = m_viewportState.viewportCount;
	for(uint i=0; i<m_viewportState.viewportCount; ++i){
		value["viewports"][i]["x"] = m_viewportState.pViewports[i].x;
		value["viewports"][i]["y"] = m_viewportState.pViewports[i].y;
		value["viewports"][i]["width"] = m_viewportState.pViewports[i].width;
		value["viewports"][i]["height"] = m_viewportState.pViewports[i].height;
		value["viewports"][i]["minDepth"] = m_viewportState.pViewports[i].minDepth;
		value["viewports"][i]["maxDepth"] = m_viewportState.pViewports[i].maxDepth;
	}
	value["scissorCount"] = m_viewportState.scissorCount;
	for(uint32_t i=0; i<m_viewportState.scissorCount; ++i){
		value["scissors"][i]["offset"]["x"] = m_viewportState.pScissors[i].offset.x;
		value["scissors"][i]["offset"]["y"] = m_viewportState.pScissors[i].offset.y;
		value["scissors"][i]["extent"]["width"] = m_viewportState.pScissors[i].extent.width;
		value["scissors"][i]["extent"]["height"] = m_viewportState.pScissors[i].extent.height;
	}
	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}

void TKPipeline::debugRasterizationState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["depthClampEnable"] = m_rasterState.depthClampEnable;
	value["rasterDiscardEnable"] = m_rasterState.rasterizerDiscardEnable;
	value["polygonMode"] = m_rasterState.polygonMode;
	value["cullMode"] = m_rasterState.cullMode;
	value["frontFace"] = m_rasterState.frontFace;
	value["depthBiasEnable"] = m_rasterState.depthBiasEnable;
	value["depthBiasConstantFactor"] = m_rasterState.depthBiasClamp;
	value["depthBiasSlopeFactor"] = m_rasterState.depthBiasSlopeFactor;
	value["lineWidth"] = m_rasterState.lineWidth;
	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}

void TKPipeline::debugMultiSampleState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["rasterizationSample"] = m_mulSampleState.rasterizationSamples;
	value["sampleShadingEnable"] = m_mulSampleState.sampleShadingEnable;
	value["minSampleShading"] = m_mulSampleState.minSampleShading;
	value["alphaToCoverageEnable"] = m_mulSampleState.alphaToCoverageEnable;
	value["alphaToOneEnable"] = m_mulSampleState.alphaToOneEnable;
	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}

void TKPipeline::debugDepthStencilState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["depthTestEnable"] = m_depthStencil.depthTestEnable;
	value["depthWriteEnable"] = m_depthStencil.depthWriteEnable;
	value["depthCompareOp"] = m_depthStencil.depthCompareOp;
	value["depthBoundsTestEnable"] = m_depthStencil.depthBoundsTestEnable;
	value["stencilTestEnable"] = m_depthStencil.stencilTestEnable;
	value["minDepthBounds"] = m_depthStencil.minDepthBounds;
	value["macDepthbounds"] = m_depthStencil.maxDepthBounds;

	value["front"]["failOp"] = m_depthStencil.front.failOp;
	value["front"]["passOp"] = m_depthStencil.front.passOp;
	value["front"]["depthFailOp"] = m_depthStencil.front.depthFailOp;
	value["front"]["compareOp"] = m_depthStencil.front.compareOp;
	value["front"]["compareMask"] = m_depthStencil.front.compareMask;
	value["front"]["writeMask"] = m_depthStencil.front.writeMask;
	value["front"]["reference"] = m_depthStencil.front.reference;

	value["back"]["failOp"] = m_depthStencil.front.failOp;
	value["back"]["passOp"] = m_depthStencil.front.passOp;
	value["back"]["depthFailOp"] = m_depthStencil.front.depthFailOp;
	value["back"]["compareOp"] = m_depthStencil.front.compareOp;
	value["back"]["compareMask"] = m_depthStencil.front.compareMask;
	value["back"]["writeMask"] = m_depthStencil.front.writeMask;
	value["back"]["reference"] = m_depthStencil.front.reference;
	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}

void TKPipeline::debugColorBlendState(){
	Json::Value value;
	value["logicOpEnable"] = m_colorBlendState.logicOpEnable;
	value["logicOp"] = m_colorBlendState.logicOp;
	value["attchmentCount"] = m_colorBlendState.attachmentCount;
	for(uint32_t i=0; i<m_colorBlendState.attachmentCount; ++i){
		value["attachments"][i]["blendEnable"] = m_colorBlendState.pAttachments[i].blendEnable;
		value["attachments"][i]["srcColorBlendFactor"] = m_colorBlendState.pAttachments[i].srcColorBlendFactor;
		value["attachments"][i]["dstColorBlendFactor"] = m_colorBlendState.pAttachments[i].dstColorBlendFactor;
		value["attachments"][i]["colorBlendOp"] = m_colorBlendState.pAttachments[i].colorBlendOp;
		value["attachments"][i]["srcAlphaBlendFactor"] = m_colorBlendState.pAttachments[i].srcAlphaBlendFactor;
		value["attachments"][i]["dstAlphaBlendFactor"] = m_colorBlendState.pAttachments[i].dstAlphaBlendFactor;
		value["attachments"][i]["alphaBlendOp"] = m_colorBlendState.pAttachments[i].alphaBlendOp;
		value["attachments"][i]["colorWriteMask"] = m_colorBlendState.pAttachments[i].colorWriteMask;
	}
	value["blendConstant"][0] = m_colorBlendState.blendConstants[0];
	value["blendConstant"][1] = m_colorBlendState.blendConstants[1];
	value["blendConstant"][2] = m_colorBlendState.blendConstants[2];
	value["blendConstant"][3] = m_colorBlendState.blendConstants[3];
	Json::StyledWriter writer;
	std::string str = writer.write(value);
	TKLog("\n%s\n", str.c_str());
}


