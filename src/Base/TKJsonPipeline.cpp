//
//TKJsonPipeline.cpp
//2019-2-22

#include "TKJsonPipeline.h"

TKJsonPipeline::TKJsonPipeline(){
    m_pipeline = VK_NULL_HANDLE;
}

TKJsonPipeline::~TKJsonPipeline(){
    TKLog("destruct pipeline object %p!\n", this);
    delete m_pipelineLayout;
    if(m_pipeline != VK_NULL_HANDLE){
        vkDestroyPipeline(VK_INFO->device, m_pipeline, nullptr);
    }
}

TKJsonPipeline *TKJsonPipeline::createFromJson(const char *fileName){
    TKJsonPipeline *pipeline = new TKJsonPipeline;
    if(pipeline->initWithJsonFile(fileName)==false){
        delete pipeline;
        pipeline = nullptr;
    }
    return pipeline;
}

bool TKJsonPipeline::initWithJsonFile(const char *fileName){
    char tmpBuf[512];
    memset(tmpBuf, 0, sizeof(char)*512);
    sprintf(tmpBuf, "%s/Json/%s", PROJECT_PATH, fileName);
    FILE *fp = fopen(tmpBuf, "rt");
    if(fp == nullptr){
        TKLog("Open file %s failed!\n", fileName);
        return false;
    }
    char tmp;
    uint64_t size = 1;
    while(fread(&tmp, sizeof(char), 1, fp)==1){
        ++ size;
    }
    char buf[size];
    fseek(fp, 0, SEEK_SET);
    fread(buf, sizeof(char), size, fp);
    std::string content(buf);
	
    Json::Value root;
    Json::Reader reader;
    if(reader.parse(content, root, true) == false){
        TKLog("Json file has error! check the file %s\n", fileName);
        return false;
    }

	TKLog("init pipeline from json start!\n");
	m_name = root["name"].asString();	
    this->initShaderStagesFromJson(root["ShaderStages"]);
    this->initVertexInputStateFromJson(root["VertexInput"]);
    this->initAssembleStateFromJson(root["InputAssemble"]);
    this->initTesslateStateFromJson(root["Tessellation"]);
    this->initViewportStateFromJson(root["Viewport"]);
    this->initRasterizationFromJson(root["Rasterization"]);
    this->initColorBlendStateFromJson(root["ColorBlend"]);
    this->initDepthStencilStateFromJson(root["DepthStencil"]);
    this->initMultiSampleStateFromJson(root["MultiSample"]);
    this->initDynamicStateFromJson(root["DynamicState"]);
    this->initPipelineLayoutFromJson(root["layout"]); 

	VkGraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext               = nullptr;
    pipelineInfo.flags               = 0;
    pipelineInfo.stageCount          = m_shaderStages.size();
    pipelineInfo.pStages             = m_shaderStages.data();
    pipelineInfo.pVertexInputState   = &m_vertInputState;
	pipelineInfo.pInputAssemblyState = &m_assembleState;
    pipelineInfo.pTessellationState  = &m_tesslState;
	pipelineInfo.pViewportState      = &m_viewportState;
    pipelineInfo.pRasterizationState = &m_rasterState;
    pipelineInfo.pMultisampleState   = &m_mulSampleState;
	pipelineInfo.pDepthStencilState  = &m_depthStencil;
	pipelineInfo.pColorBlendState    = &m_colorBlendState;
	pipelineInfo.pDynamicState       = &m_dynamicState;
    pipelineInfo.layout              = m_pipelineLayout->pipelineLayout();
    pipelineInfo.renderPass          = VK_INFO->renderPass;
    pipelineInfo.subpass             = 0;
    pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex   = -1;
	
    VkResult ret = vkCreateGraphicsPipelines(VK_INFO->device, VK_NULL_HANDLE,
                                             1, &pipelineInfo, nullptr, &m_pipeline);
	if (ret != VK_SUCCESS) {
        printf("init pipeline failed! ret = %d\n", ret);
        return false;
    }
	TKLog("init pipeline %s success\n", m_name.c_str());

	return true;
}

bool TKJsonPipeline::initShaderStagesFromJson(const Json::Value &value){
    if(value.isArray() == false){
        TKLog("Load shader stage error!\n");
        return false;
    }

	m_stageName.clear();
    m_shaderStages.resize(value.size());
    for(uint i=0; i<value.size(); ++i){
        m_shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_shaderStages[i].pNext = nullptr;
		m_shaderStages[i].flags = 0;
		uint32_t stage = value[i]["stage"].asUInt();
		switch(stage){
		case 1:
			m_shaderStages[i].stage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case 5:
			m_shaderStages[i].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		default:
			
			break;
		}

		std::string moduleStr = value[i]["module"].asString();
		m_shaderStages[i].module = TKModuleManager::shaderModule(moduleStr.c_str());
		if(m_shaderStages[i].module == VK_NULL_HANDLE){
			TKLog("shader module error!\n");
		}
		m_shaderStages[i].pName = "main";
        m_shaderStages[i].pSpecializationInfo = nullptr;
	}

	return true;
}

bool TKJsonPipeline::initVertexInputStateFromJson(const Json::Value &value){
    m_vertInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    m_vertInputState.pNext = nullptr;
    m_vertInputState.flags = 0;
	printf("%s\n", value.toStyledString().c_str());
    Json::Value bindValues = value["VertexBindDesc"];
    if(bindValues.isArray() == false){
        return false;
    }
    Json::Value attrValues = value["VertexAttribDesc"];
    if(attrValues.isArray() == false){
        return false;
    }
    m_inputBindDesc.resize(bindValues.size());
    m_inputAttribDesc.resize(attrValues.size());
    for(uint32_t i=0; i<bindValues.size(); ++i){
		m_inputBindDesc[i].binding = bindValues[i]["binding"].asUInt();
        m_inputBindDesc[i].stride  = bindValues[i]["stride"].asUInt();
        std::string inputRateStr = bindValues[i]["inputRate"].asString();
        m_inputBindDesc[i].inputRate = TKVkUtility::VkInputRateFrom(inputRateStr);
    }
 
    for(uint32_t i=0; i<attrValues.size(); ++i){
        m_inputAttribDesc[i].binding  = attrValues[i]["binding"].asUInt();
        m_inputAttribDesc[i].location = attrValues[i]["location"].asUInt();
        m_inputAttribDesc[i].offset   = attrValues[i]["offset"].asUInt();
        std::string formatStr         = attrValues[i]["format"].asString();
        m_inputAttribDesc[i].format   = TKVkUtility::VkFormatFrom(formatStr);
    }
    
	m_vertInputState.vertexBindingDescriptionCount = m_inputBindDesc.size();
    m_vertInputState.pVertexBindingDescriptions = m_inputBindDesc.data();
	printf("attribute desc count: %lu\n", m_inputAttribDesc.size());
    m_vertInputState.vertexAttributeDescriptionCount = m_inputAttribDesc.size();
    m_vertInputState.pVertexAttributeDescriptions = m_inputAttribDesc.data();
	
	this->debugVertexInputState();
	return true;
}

bool TKJsonPipeline::initAssembleStateFromJson(const Json::Value &value){
    m_assembleState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_assembleState.pNext = nullptr;
    m_assembleState.flags = 0;
	
    m_assembleState.primitiveRestartEnable = value["primitiveRestart"].asBool();
    std::string topologyStr = value["topology"].asString();
	m_assembleState.topology = TKVkUtility::VkPrimitiveFrom(topologyStr);

	return true;
}

bool TKJsonPipeline::initViewportStateFromJson(const Json::Value &value){
    m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    m_viewportState.pNext = nullptr;
    m_viewportState.flags = 0;
	
    if(value["pViewports"].isArray()==true){
        m_viewports.resize(value["pViewports"].size());
        for(uint32_t i=0; i<value["pViewports"].size(); ++i){
            Json::Value viewport = value["pViewports"][i];
            m_viewports[i].x = viewport["x"].asFloat();
            m_viewports[i].y = viewport["y"].asFloat();
            m_viewports[i].width  = viewport["w"].asFloat();
            m_viewports[i].height = viewport["h"].asFloat();
            m_viewports[i].minDepth = viewport["minDepth"].asFloat();
            m_viewports[i].maxDepth = viewport["maxDepth"].asFloat();
        }
    }
    if(value["pScissors"].isArray()==true){
        m_scissorRects.resize(value["pScissors"].size());
        for(uint32_t i=0; i<value["pScissors"].size(); ++i){
            Json::Value scissor = value["pScissors"][i];
            m_scissorRects[i].extent.width = scissor["extent"]["w"].asFloat();
            m_scissorRects[i].extent.height= scissor["extent"]["h"].asFloat();
            m_scissorRects[i].offset.x = scissor["offset"]["x"].asFloat();
            m_scissorRects[i].offset.y = scissor["offset"]["y"].asFloat();
        }
    }
    
    m_viewportState.viewportCount = m_viewports.size();
    m_viewportState.pViewports    = m_viewports.data();
    m_viewportState.scissorCount  = m_scissorRects.size();
    m_viewportState.pScissors     = m_scissorRects.data();

	return true;
}

bool TKJsonPipeline::initTesslateStateFromJson(const Json::Value &value){
    m_tesslState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    m_tesslState.pNext = nullptr;
    m_tesslState.flags = 0;
    m_tesslState.patchControlPoints = value["patchControlPoints"].asUInt();
    return true;
}

bool TKJsonPipeline::initRasterizationFromJson(const Json::Value &value){
    m_rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_rasterState.pNext = nullptr;
    m_rasterState.flags = 0;
	
    m_rasterState.depthClampEnable = TKVkUtility::VkBoolFrom(value["depthClampEnable"].asString());
    m_rasterState.rasterizerDiscardEnable = TKVkUtility::VkBoolFrom(value["rasterDiscard"].asString());
    m_rasterState.depthBiasEnable = TKVkUtility::VkBoolFrom(value["depthBiasEnable"].asString());
    m_rasterState.lineWidth       = value["lineWidth"].asFloat();
    m_rasterState.depthBiasConstantFactor = value["depthBiasConstantFactor"].asFloat();
    m_rasterState.depthBiasClamp          = value["depthBiasClamp"].asFloat();
    m_rasterState.depthBiasSlopeFactor    = value["depthBiasSlopeFactor"].asFloat();
    m_rasterState.cullMode    = TKVkUtility::VkCullModeFrom(value["cullMode"].asString());
    m_rasterState.polygonMode = TKVkUtility::VkPolygonModeFrom(value["polygonMode"].asString());
    m_rasterState.frontFace   = TKVkUtility::VkFrontFaceFrom(value["frontFace"].asString());
    
	return true;
}

bool TKJsonPipeline::initMultiSampleStateFromJson(const Json::Value &value){
    m_mulSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_mulSampleState.pNext = nullptr;
    m_mulSampleState.flags = 0;
	
	std::string str = value["rasterizationSamples"].asString();
    m_mulSampleState.rasterizationSamples = TKVkUtility::VkSampleCountFlagBitFrom(str);
    m_mulSampleState.sampleShadingEnable = TKVkUtility::VkBoolFrom(value["sampleShadingEnable"].asString());
    m_mulSampleState.minSampleShading = value["minSampleShading"].asFloat();
    m_mulSampleState.alphaToCoverageEnable = TKVkUtility::VkBoolFrom(value["alphaToCoverageEnable"].asString());
    m_mulSampleState.alphaToOneEnable = TKVkUtility::VkBoolFrom(value["alphaToOneEnable"].asString());
    
	uint32_t sampleMask[] = {0x1};
	m_mulSampleState.pSampleMask = sampleMask;
        
    return true;
}

bool TKJsonPipeline::initDepthStencilStateFromJson(const Json::Value &value){
    m_depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    m_depthStencil.pNext                 = nullptr;
    m_depthStencil.flags                 = 0;
	
    m_depthStencil.depthTestEnable       = TKVkUtility::VkBoolFrom(value["depthTestEnable"].asString());
    m_depthStencil.depthWriteEnable      = TKVkUtility::VkBoolFrom(value["depthWriteEnable"].asString());
    m_depthStencil.depthCompareOp        = TKVkUtility::VkCompareOpFrom(value["depthCompareOp"].asString());
    m_depthStencil.depthBoundsTestEnable = TKVkUtility::VkBoolFrom(value["depthBoundsTestEnable"].asString());
    m_depthStencil.front.failOp          = TKVkUtility::VkStencilOpFrom(value["front"]["failOp"].asString());
    m_depthStencil.front.passOp          = TKVkUtility::VkStencilOpFrom(value["front"]["passOp"].asString());
    m_depthStencil.front.depthFailOp     = TKVkUtility::VkStencilOpFrom(value["front"]["depthFailOp"].asString());
    m_depthStencil.front.compareOp       = TKVkUtility::VkCompareOpFrom(value["front"]["compareOp"].asString());
    m_depthStencil.front.compareMask     = value["front"]["compareMask"].asUInt();
    m_depthStencil.front.writeMask       = value["front"]["writeMask"].asUInt();
    m_depthStencil.front.reference       = value["front"]["reference"].asUInt();
    m_depthStencil.back.failOp           = TKVkUtility::VkStencilOpFrom(value["back"]["failOp"].asString());
    m_depthStencil.back.passOp           = TKVkUtility::VkStencilOpFrom(value["back"]["passOp"].asString());
    m_depthStencil.back.depthFailOp      = TKVkUtility::VkStencilOpFrom(value["back"]["depthFailOp"].asString());
    m_depthStencil.back.compareOp        = TKVkUtility::VkCompareOpFrom(value["back"]["compareOp"].asString());
    m_depthStencil.back.compareMask      = value["back"]["compareMask"].asUInt();
    m_depthStencil.back.writeMask        = value["back"]["writeMask"].asUInt();
    m_depthStencil.back.reference        = value["back"]["reference"].asUInt();
    m_depthStencil.stencilTestEnable     = TKVkUtility::VkBoolFrom(value["stencilTestEnable"].asString());
    m_depthStencil.maxDepthBounds        = value["maxDepthBounds"].asFloat();
    m_depthStencil.minDepthBounds        = value["minDepthBounds"].asFloat();
	
    return true;
}

bool TKJsonPipeline::initColorBlendStateFromJson(const Json::Value &value){
    m_colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_colorBlendState.pNext = nullptr;
    m_colorBlendState.flags = 0;
	
	m_colorBlendState.logicOpEnable      = TKVkUtility::VkBoolFrom(value["logicOpEnable"].asString());
    m_colorBlendState.logicOp            = TKVkUtility::VkLogicOpFrom(value["logicOp"].asString());
    if(value["pAttachments"].isArray()==true){
        m_blendAttachArr.resize(value["pAttachments"].size());
        for(uint32_t i=0; i<value["pAttachments"].size(); ++i){
            Json::Value tmp = value["pAttachments"][i];
            m_blendAttachArr[i].blendEnable = TKVkUtility::VkBoolFrom(tmp["blendEnable"].asString());
            std::string srcColorFactor = tmp["srcColorBlendFactor"].asString();
            m_blendAttachArr[i].srcColorBlendFactor = TKVkUtility::VkBlendFactorFrom(srcColorFactor);
            std::string dstColorFactor = tmp["dstColorBlendFactor"].asString();
            m_blendAttachArr[i].dstColorBlendFactor = TKVkUtility::VkBlendFactorFrom(dstColorFactor);
            std::string srcAlphaFactor = tmp["srcAlphaBlendFactor"].asString();
            m_blendAttachArr[i].srcAlphaBlendFactor = TKVkUtility::VkBlendFactorFrom(srcAlphaFactor);
			std::string dstAlphaFactor = tmp["dstAlphaBlendFactor"].asString();
            m_blendAttachArr[i].dstAlphaBlendFactor = TKVkUtility::VkBlendFactorFrom(dstAlphaFactor);
			m_blendAttachArr[i].colorBlendOp = TKVkUtility::VkBlendOpFrom(tmp["colorBlendOp"].asString());
			m_blendAttachArr[i].alphaBlendOp = TKVkUtility::VkBlendOpFrom(tmp["alphaBlendOp"].asString());           
			m_blendAttachArr[i].colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|
				VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT;
		}
    }
    m_colorBlendState.attachmentCount    = m_blendAttachArr.size();
    m_colorBlendState.pAttachments       = m_blendAttachArr.data();
    m_colorBlendState.blendConstants[0] = 0.0;
    m_colorBlendState.blendConstants[1] = 0.0;
    m_colorBlendState.blendConstants[2] = 0.0;
    m_colorBlendState.blendConstants[3] = 0.0;
    if(value["blendConstants"].isArray()==true){
        m_colorBlendState.blendConstants[0] = value["blendConstants"][0].asFloat();
        m_colorBlendState.blendConstants[1] = value["blendConstants"][1].asFloat();
        m_colorBlendState.blendConstants[2] = value["blendConstants"][2].asFloat();
        m_colorBlendState.blendConstants[3] = value["blendConstants"][3].asFloat();
    }
    
    return true;
}

bool TKJsonPipeline::initDynamicStateFromJson(const Json::Value &value){
    m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_dynamicState.pNext = nullptr;
    m_dynamicState.flags = 0;
    m_dynamicStates.clear();
	
	//TKLog("dynamic State : %s\n", value.toStyledString().c_str());
    if(value.isArray()==true){
        m_dynamicStates.resize(value.size());
        for(uint32_t i=0; i<value.size(); ++i){
			//TKLog("\n%s\n", value[i].asString().c_str());
            m_dynamicStates[i] = TKVkUtility::VkDynamicStateFrom(value[i].asString());
        }
		m_dynamicState.dynamicStateCount  = m_dynamicStates.size();
		m_dynamicState.pDynamicStates     = m_dynamicStates.data();
	}
	
	m_dynamicState.dynamicStateCount  = m_dynamicStates.size();
    m_dynamicState.pDynamicStates     = m_dynamicStates.data();
    return false;
}

bool TKJsonPipeline::initPipelineLayoutFromJson(const Json::Value &value){
	m_pipelineLayout = TKPipelineLayout::createLayoutFromJson(value);
	if(m_pipelineLayout == nullptr){
		return false;
	}
	TKLog("pipeline layout %p\n", m_pipelineLayout->pipelineLayout());
	return true;
}

VkPipeline TKJsonPipeline::pipeline() const {
    return m_pipeline;
}

VkPipelineLayout TKJsonPipeline::pipelineLayout() const {
    return m_pipelineLayout->pipelineLayout();
}

VkDescriptorSet TKJsonPipeline::descriptorSet(uint32_t idx) const{
    return m_pipelineLayout->descriptorSet(idx);
}

const TKPipelineLayout *TKJsonPipeline::getUsingTkPipelineLayout(){
    return m_pipelineLayout;
}

std::string TKJsonPipeline::pipelineName() const{
    return m_name;
}

void TKJsonPipeline::debugShaderStageState(){
	Json::Value value;
	for(uint32_t i=0; i<m_shaderStages.size(); ++i){
		value[i]["stage"] = m_shaderStages[i].stage;
		value[i]["pName"] = m_shaderStages[i].pName;
	}
	Json::StyledWriter writer;
	std::string str = writer.write(value);
	m_debugInfoJson["shaderStage"] = value;
}

void TKJsonPipeline::debugVertexInputState(){
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
	m_debugInfoJson["vertexInput"] = value;
}

void TKJsonPipeline::debugInputAssemblyState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["topology"] = m_assembleState.topology;
	value["primitive"] = m_assembleState.primitiveRestartEnable;

	std::string str = writer.write(value);
	m_debugInfoJson["inputAssemble"] = value;
}

void TKJsonPipeline::debugTessellationState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["patchControlPoints"] = m_tesslState.patchControlPoints;
	std::string str = writer.write(value);
	m_debugInfoJson["tessellation"] = value;
}

void TKJsonPipeline::debugViewportState(){
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
	m_debugInfoJson["viewport"] = value;
}

void TKJsonPipeline::debugRasterizationState(){
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
	m_debugInfoJson["rasterization"] = value;
}

void TKJsonPipeline::debugMultiSampleState(){
	Json::Value value;
	Json::StyledWriter writer;
	value["rasterizationSample"] = m_mulSampleState.rasterizationSamples;
	value["sampleShadingEnable"] = m_mulSampleState.sampleShadingEnable;
	value["minSampleShading"] = m_mulSampleState.minSampleShading;
	value["alphaToCoverageEnable"] = m_mulSampleState.alphaToCoverageEnable;
	value["alphaToOneEnable"] = m_mulSampleState.alphaToOneEnable;
	std::string str = writer.write(value);
	m_debugInfoJson["multiSample"] = value;
}

void TKJsonPipeline::debugDepthStencilState(){
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
	m_debugInfoJson["depthStencil"] = value;
}

void TKJsonPipeline::debugColorBlendState(){
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
	m_debugInfoJson["coloeBlend"] = value;
}

void TKJsonPipeline::debugDynamicState(){
	Json::Value value;
	for(uint32_t i=0; i<m_dynamicState.dynamicStateCount; ++i){
		value[i] = m_dynamicState.pDynamicStates[i];
	}
	Json::StyledWriter writer;
	std::string str = writer.write(value);
	m_debugInfoJson["dynamic"] = value;
}

void TKJsonPipeline::debugPipeline(){
	this->debugShaderStageState();
	this->debugVertexInputState();
	this->debugInputAssemblyState();
	this->debugTessellationState();
	this->debugViewportState();
	this->debugRasterizationState();
	this->debugMultiSampleState();
	this->debugDepthStencilState();
	this->debugColorBlendState();
	this->debugDynamicState();
	Json::StyledWriter writer;
	std::string str = writer.write(m_debugInfoJson);
	TKLog("pipeline %s info:\n%s\n", m_name.c_str(), str.c_str());
}
