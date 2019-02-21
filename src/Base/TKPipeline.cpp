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
    }

    if(tessCtlFile != nullptr){
        m_moduleMap["tess_ctrl"] = tessCtlFile;
    }

    if(tessEvaFile != nullptr){
        m_moduleMap["tess_evaluation"] = tessEvaFile;
    }
    
    if(false == this->initShaderStageCreateInfo()){
        return false;
    }
    this->initDynamicState();
	
    return true;
}

TKPipeline *TKPipeline::createWithJson(const char *fileName){
    TKPipeline *pipeline = new TKPipeline;
    if(pipeline->initWithJson(fileName)==false){
        delete pipeline;
        pipeline = nullptr;
    }
    return pipeline;
}

bool TKPipeline::initWithJson(const char *fileName){
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
    this->initPipeline(root["name"].asString());	
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
    m_geomModule = TKModuleManager::shaderModule(m_moduleMap["geometry"].c_str());
    m_tessCtlModule = TKModuleManager::shaderModule(m_moduleMap["tess_ctrl"].c_str());
    m_tessEvlModule = TKModuleManager::shaderModule(m_moduleMap["tess_evaluation"].c_str());
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

bool TKPipeline::initShaderStagesFromJson(const Json::Value &value){
    if(value.isArray() == false){
        TKLog("Load shader stage error!\n");
        return false;
    }
	
    m_shaderStages.resize(value.size());
    for(uint i=0; i<value.size(); ++i){
		Json::Value subValue = value[i];
		std::string module = subValue["module"].asString();
        m_shaderStages[i].flags = 0;
        m_shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_shaderStages[i].pNext = nullptr;
        m_shaderStages[i].module = TKModuleManager::shaderModule(module.c_str());
        std::string stageStr = subValue["stage"].asString();
        m_shaderStages[i].stage = TKVkUtility::VkShaderStageFlagBitsFrom(stageStr.c_str());
		m_shaderStages[i].pName = subValue["pName"].asString().c_str();
        m_shaderStages[i].pSpecializationInfo = nullptr;
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

bool TKPipeline::initVertexInputStateFromJson(const Json::Value &value){
    m_vertInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    m_vertInputState.pNext = nullptr;
    m_vertInputState.flags = 0;
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
    m_vertInputState.vertexAttributeDescriptionCount = m_inputAttribDesc.size();
    m_vertInputState.pVertexAttributeDescriptions = m_inputAttribDesc.data();
    return true;
}

void TKPipeline::initAssembleState(const VkPrimitiveTopology toplogy, const VkBool32 restartEnable){
    m_assembleState.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_assembleState.pNext    = nullptr;
    m_assembleState.flags    = 0;
    m_assembleState.topology = toplogy;
    m_assembleState.primitiveRestartEnable = restartEnable;
}

bool TKPipeline::initAssembleStateFromJson(const Json::Value &value){
    m_assembleState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_assembleState.pNext = nullptr;
    m_assembleState.flags = 0;
    m_assembleState.primitiveRestartEnable = value["primitiveRestart"].asBool();
    std::string topologyStr = value["topology"].asString();
    m_assembleState.topology = TKVkUtility::VkPrimitiveFrom(topologyStr);
    return true;
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

bool TKPipeline::initViewportStateFromJson(const Json::Value &value){
    m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    m_viewportState.pNext = nullptr;
    m_viewportState.flags = 0;
    if(value["pViewports"].isArray()==true){
        m_viewports.resize(value["pViewports"].size());
        for(int i=0; i<value["pViewports"].size(); ++i){
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
        for(int i=0; i<value["pScissors"].size(); ++i){
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

void TKPipeline::initTessllationState(uint32_t patchPoints){
    m_tesslState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    m_tesslState.flags = 0;
    m_tesslState.pNext = nullptr;
    m_tesslState.patchControlPoints = patchPoints;
}

bool TKPipeline::initTesslateStateFromJson(const Json::Value &value){
    m_tesslState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    m_tesslState.pNext = nullptr;
    m_tesslState.flags = 0;
    m_tesslState.patchControlPoints = value["patchControlPoints"].asUInt();
    return true;
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

bool TKPipeline::initRasterizationFromJson(const Json::Value &value){
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

bool TKPipeline::initMultiSampleStateFromJson(const Json::Value &value){
    m_mulSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_mulSampleState.pNext = nullptr;
    m_mulSampleState.flags = 0;
    std::string str = value["rasterizationSamples"].asString();
    m_mulSampleState.rasterizationSamples = TKVkUtility::VkSampleCountFlagBitFrom(str);
    m_mulSampleState.sampleShadingEnable = TKVkUtility::VkBoolFrom(value["sampleShadingEnable"].asString());
    m_mulSampleState.minSampleShading = value["minSampleShading"].asFloat();
    m_mulSampleState.alphaToCoverageEnable = TKVkUtility::VkBoolFrom(value["alphaToCoverageEnable"].asString());
    m_mulSampleState.alphaToOneEnable = TKVkUtility::VkBoolFrom(value["alphaToOneEnable"].asString());
    if(value["pSampleMask"].isArray()==true){
        if(value["pSampleMask"].size()>0){
            std::vector<VkSampleMask> sampleMasks;
            for(int i=0; i<value["pSampleMask"].size(); ++i){
                sampleMasks.push_back(value["pSampleMask"][i].asUInt());
            }
            m_mulSampleState.pSampleMask = sampleMasks.data();
        }else{
            m_mulSampleState.pSampleMask = nullptr;
        }
    }else{
        m_mulSampleState.pSampleMask = nullptr;
    }    
    return true;
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

bool TKPipeline::initDepthStencilStateFromJson(const Json::Value &value){
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
    m_depthStencil.maxDepthBounds        = value["minDepthBounds"].asFloat();
    m_depthStencil.minDepthBounds        = value["maxDepthBounds"].asFloat();
    return true;
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
bool TKPipeline::initColorBlendStateFromJson(const Json::Value &value){
    m_colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_colorBlendState.pNext = nullptr;
    m_colorBlendState.flags = 0;
    m_colorBlendState.logicOpEnable      = TKVkUtility::VkBoolFrom(value["logicOpEnable"].asString());
    m_colorBlendState.logicOp            = TKVkUtility::VkLogicOpFrom(value["logicOp"].asString());
    if(value["pAttachments"].isArray()==true){
        m_blendAttachArr.resize(value["pAttachments"].size());
        for(int i=0; i<value["pAttachments"].size(); ++i){
            Json::Value tmp = value["pAttachments"][i];
            m_blendAttachArr[i].blendEnable = TKVkUtility::VkBoolFrom(tmp["blendEnable"].asString());
            std::string srcColorFactor = tmp["srcColorBlendFactor"].asString();
            m_blendAttachArr[i].srcColorBlendFactor = TKVkUtility::VkBlendFactorFrom(srcColorFactor);
            std::string dstColorFactor = tmp["dstColorBlendFactor"].asString();
            m_blendAttachArr[i].dstColorBlendFactor = TKVkUtility::VkBlendFactorFrom(dstColorFactor);
            m_blendAttachArr[i].colorBlendOp = TKVkUtility::VkBlendOpFrom(tmp["colorBlendOp"].asString());

            std::string srcAlphaFactor = tmp["srcAlphaBlendFactor"].asString();
            m_blendAttachArr[i].srcAlphaBlendFactor = TKVkUtility::VkBlendFactorFrom(srcAlphaFactor);
            std::string dstAlphaFactor = tmp["dstAlphaBlendFactor"].asString();
            m_blendAttachArr[i].dstAlphaBlendFactor = TKVkUtility::VkBlendFactorFrom(dstAlphaFactor);
            m_blendAttachArr[i].alphaBlendOp = TKVkUtility::VkBlendOpFrom(tmp["alphaBlendOp"].asString());

            if(tmp["colorWriteMask"].isArray()==true){
                std::string str = tmp["colorWriteMask"][0].asString();
                VkColorComponentFlagBits flag = TKVkUtility::VkColorComponentFlagBitFrom(str);
                for(int j=1; j<tmp["colorWriteMask"].size(); ++j){
                    str = tmp["colorWriteMask"][j].asString();
                    flag || TKVkUtility::VkColorComponentFlagBitFrom(str);
                }
                m_blendAttachArr[i].colorWriteMask = flag;
            }else{
                m_blendAttachArr[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|
                    VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT;
            }
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

bool TKPipeline::initDynamicStateFromJson(const Json::Value &value){
    m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_dynamicState.pNext = nullptr;
    m_dynamicState.flags = 0;
    m_dynamicStates.clear();
    if(value["dynamicState"].isArray()==true){
        m_dynamicStates.resize(value["dynamicState"].size());
        for(int i=0; i<value["dynamicState"].size(); ++i){
            m_dynamicStates[i] = TKVkUtility::VkDynamicStateFrom(value["dynamicState"][i].asString());
        }
		m_dynamicState.dynamicStateCount  = m_dynamicStates.size();
		m_dynamicState.pDynamicStates     = m_dynamicStates.data();
	}
    return false;
}

void TKPipeline::initPipelineLayout(const std::string &name){
    m_pipelineLayout = TKPipelineLayout::createPipelineLayout(name);
}

void TKPipeline::initPipelineLayoutFromJson(const Json::Value &value){
	m_pipelineLayout = TKPipelineLayout::createLayoutFromJson(value);
}


TKPipeline *TKPipeline::createComputePipeline(const std::string &name){
    TKPipeline *pipeline = new TKPipeline();
    if(false == pipeline->initComputePipeline(name)){
        delete pipeline;
        pipeline = nullptr;
    }
    return pipeline;
}

bool TKPipeline::initComputePipeline(const std::string &name) {
    m_name = name;
    return true;
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





