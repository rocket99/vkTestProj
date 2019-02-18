//
//TKPipelineManager.cpp
//2018-11-27

#include "TKPipelineManager.h"
#include "TKBaseInfo.h"

static TKPipelineManager *instance = nullptr;
TKPipelineManager::TKPipelineManager(){}
TKPipelineManager::~TKPipelineManager(){
    pipelineDict.clear();
    for(int i=0; i<allPipelines.size(); ++i){
        delete allPipelines[i];
    }
    allPipelines.clear();
}

TKPipelineManager* TKPipelineManager::manager(){
    if(instance == nullptr){
        instance = new TKPipelineManager();
    }
    return instance;
}

void TKPipelineManager::purge(){
    if(instance != nullptr){
        delete instance;
    }
    instance = nullptr;
}

void TKPipelineManager::prepareAllPipelines(){
    //TKPipelineManager::manager()->prepareFirstPipeline();
    TKPipelineManager::manager()->prepareBasicPipeline();
    //TKPipelineManager::manager()->prepareLightPipeline();
    TKPipelineManager::manager()->preparePipelineFromJson("light_pipeline.json");
}

void TKPipelineManager::preparePipelineFromJson(const char *fileName){
    TKPipeline *pipeline = TKPipeline::createWithJson(fileName);
    if(pipeline != nullptr){
        std::string pipelineName = pipeline->pipelineName();
        TKPipelineManager::manager()->allPipelines.push_back(pipeline);
        TKPipelineManager::manager()->pipelineDict[pipelineName] = pipeline;
    }
}

void TKPipelineManager::addNewPipeline(TKPipeline *pipeline, std::string name){
    if(pipeline == nullptr){
        return;
    }
    TKPipelineManager::manager()->pipelineDict[name] = pipeline;
}

VkPipeline TKPipelineManager::getPipeline(const std::string &pipelineName){
    std::map<std::string, TKPipeline *>::iterator iter;
    iter = TKPipelineManager::manager()->pipelineDict.find(pipelineName);
    if(iter == TKPipelineManager::manager()->pipelineDict.end()){
        return VK_NULL_HANDLE;
    }
    return ((TKPipeline *)(iter->second))->pipeline();
}

TKPipeline *TKPipelineManager::getTKPipeline(const std::string &pipelineName){
    std::map<std::string, TKPipeline *>::iterator iter;
    iter = TKPipelineManager::manager()->pipelineDict.find(pipelineName);
    if(iter == TKPipelineManager::manager()->pipelineDict.end()){
        return nullptr;
    }
    return (iter->second);
}

void TKPipelineManager::prepareBasicPipeline(){
    TKPipeline *tmpPipeline = TKPipeline::createGraphicPipeline("basic_vert.spv", "basic_frag.spv");
    const std::vector<VkVertexInputBindingDescription> bindDesc= {
        {0, 12*sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX}
    };
    const std::vector<VkVertexInputAttributeDescription> attribDesc = {
        {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
        {1, 0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)*3},
        {2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(float)*6},
        {3, 0, VK_FORMAT_R32G32_SFLOAT, sizeof(float)*10}
    };
    tmpPipeline->initVertexInputState(bindDesc, attribDesc);
    tmpPipeline->initAssembleState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);
    tmpPipeline->initTessllationState(0);
    std::vector<VkViewport> vp = {
       {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0}
    };
    std::vector<VkRect2D> sc = {
        { {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT} }
    };
    tmpPipeline->initViewportState(vp, sc);
    tmpPipeline->initRasterState(VK_TRUE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
                                 VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 1.0, 0.0, 0.0, 1.0);
    uint32_t sampleMask[] = { 0x1 };
    tmpPipeline->initMultipleSampleState(VK_SAMPLE_COUNT_1_BIT, VK_FALSE,
                                         1.0, sampleMask, VK_FALSE, VK_FALSE);

    VkStencilOpState stencilOpState;
    stencilOpState.failOp      = VK_STENCIL_OP_KEEP;
    stencilOpState.passOp      = VK_STENCIL_OP_KEEP;
    stencilOpState.depthFailOp = VK_STENCIL_OP_KEEP;
    stencilOpState.compareOp   = VK_COMPARE_OP_ALWAYS;
    stencilOpState.compareMask = 0x1;
    stencilOpState.writeMask   = 0x1;
    stencilOpState.reference   = 0x0;
    tmpPipeline->initDepthStencilState(VK_FALSE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL, VK_TRUE,
                                       VK_FALSE, 0.0, 1.0, stencilOpState, stencilOpState);

    float blendConst[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    uint32_t colorAttachmentCount = TKBaseInfo::Info()->colorAttachmentCount;
    std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachState(colorAttachmentCount);
    for(int i=0; i<colorBlendAttachState.size();++i){
        colorBlendAttachState[i].blendEnable         = VK_TRUE;
        colorBlendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
        colorBlendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        colorBlendAttachState[i].colorBlendOp        = VK_BLEND_OP_ADD;
        
        colorBlendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachState[i].alphaBlendOp        = VK_BLEND_OP_ADD;
        colorBlendAttachState[i].colorWriteMask      =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }
    tmpPipeline->initColorBlendState(VK_FALSE, VK_LOGIC_OP_COPY,
                                     blendConst, colorBlendAttachState);
    tmpPipeline->initPipelineLayout("basic");
    tmpPipeline->initPipeline("basic");
    TKPipelineManager::manager()->allPipelines.push_back(tmpPipeline);
    TKPipelineManager::manager()->pipelineDict["basic"] = tmpPipeline;
}
/*
void TKPipelineManager::prepareFirstPipeline(){
    TKPipeline *tmpPipeline = TKPipeline::createGraphicPipeline("vert_fst.spv", "frag_fst.spv");
    const std::vector<VkVertexInputBindingDescription> bindDesc;
    bindDesc.empty();
    const std::vector<VkVertexInputAttributeDescription> attribDesc;
    attribDesc.empty();
    tmpPipeline->initVertexInputState(bindDesc, attribDesc);
    tmpPipeline->initAssembleState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);
    tmpPipeline->initTessllationState(0);
    std::vector<VkViewport> vp = {
       {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0}
    };
    std::vector<VkRect2D> sc = {
        { {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT} }
    };
    tmpPipeline->initViewportState(vp, sc);
    tmpPipeline->initRasterState(VK_TRUE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
                                   VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_TRUE, 1.0, 0.0, 0.0, 1.0);
    uint32_t sampleMask[] = { 0x1 };
    tmpPipeline->initMultipleSampleState(VK_SAMPLE_COUNT_1_BIT, VK_FALSE,
                                          1.0, sampleMask, VK_TRUE, VK_TRUE);

    VkStencilOpState stencilOpState;
    stencilOpState.failOp      = VK_STENCIL_OP_KEEP;
    stencilOpState.passOp      = VK_STENCIL_OP_KEEP;
    stencilOpState.depthFailOp = VK_STENCIL_OP_KEEP;
    stencilOpState.compareOp   = VK_COMPARE_OP_ALWAYS;
    stencilOpState.compareMask = 0x1;
    stencilOpState.writeMask   = 0x1;
    stencilOpState.reference   = 0x0;
    tmpPipeline->initDepthStencilState(VK_FALSE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL, VK_TRUE,
                                         VK_FALSE, 0.0, 1.0, stencilOpState, stencilOpState);
    float blendConst[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    uint32_t colorAttachmentCount = TKBaseInfo::Info()->colorAttachmentCount;
    std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachState(colorAttachmentCount);
    for(int i=0; i<colorBlendAttachState.size(); ++i){
        colorBlendAttachState[i].blendEnable         = VK_TRUE;
        colorBlendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
        colorBlendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        colorBlendAttachState[i].colorBlendOp        = VK_BLEND_OP_ADD;
        
        colorBlendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachState[i].alphaBlendOp        = VK_BLEND_OP_ADD;
        colorBlendAttachState[i].colorWriteMask      =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }
    tmpPipeline->initColorBlendState(VK_FALSE, VK_LOGIC_OP_COPY,
                                     blendConst, colorBlendAttachState);
    tmpPipeline->initPipelineLayout("first");
    tmpPipeline->initPipeline("first");
    TKPipelineManager::manager()->allPipelines.push_back(tmpPipeline);
    TKPipelineManager::manager()->pipelineDict["first"] = tmpPipeline;
}

void TKPipelineManager::prepareLightPipeline(){
    TKPipeline *tmpPipeline = TKPipeline::createGraphicPipeline("light_vert.spv", "light_frag.spv");
    const std::vector<VkVertexInputBindingDescription> bindDesc= {
        {0, 12*sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX}
    };
    const std::vector<VkVertexInputAttributeDescription> attribDesc = {
        {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
        {1, 0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)*3},
        {2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(float)*6},
        {3, 0, VK_FORMAT_R32G32_SFLOAT, sizeof(float)*10}
    };
    tmpPipeline->initVertexInputState(bindDesc, attribDesc);
    tmpPipeline->initAssembleState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);
    tmpPipeline->initTessllationState(0);
    std::vector<VkViewport> vp = {
       {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0}
    };
    std::vector<VkRect2D> sc = {
        { {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT} }
    };
    tmpPipeline->initViewportState(vp, sc);
    tmpPipeline->initRasterState(VK_TRUE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
                                 VK_FRONT_FACE_CLOCKWISE, VK_FALSE, 1.0, 0.0, 0.0, 1.0);
    uint32_t sampleMask[] = { 0x1 };
    tmpPipeline->initMultipleSampleState(VK_SAMPLE_COUNT_1_BIT, VK_FALSE,
                                         1.0, sampleMask, VK_FALSE, VK_FALSE);

    VkStencilOpState stencilOpState;
    stencilOpState.failOp      = VK_STENCIL_OP_KEEP;
    stencilOpState.passOp      = VK_STENCIL_OP_KEEP;
    stencilOpState.depthFailOp = VK_STENCIL_OP_KEEP;
    stencilOpState.compareOp   = VK_COMPARE_OP_ALWAYS;
    stencilOpState.compareMask = 0x1;
    stencilOpState.writeMask   = 0x1;
    stencilOpState.reference   = 0x0;
    tmpPipeline->initDepthStencilState(VK_FALSE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL, VK_TRUE,
                                       VK_FALSE, 0.0, 1.0, stencilOpState, stencilOpState);

    float blendConst[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    uint32_t colorAttachmentCount = TKBaseInfo::Info()->colorAttachmentCount;
    std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachState(colorAttachmentCount);
    for(int i=0; i<colorBlendAttachState.size();++i){
        colorBlendAttachState[i].blendEnable         = VK_TRUE;
        colorBlendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
        colorBlendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        colorBlendAttachState[i].colorBlendOp        = VK_BLEND_OP_ADD;
        
        colorBlendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachState[i].alphaBlendOp        = VK_BLEND_OP_ADD;
        colorBlendAttachState[i].colorWriteMask      =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }
    tmpPipeline->initColorBlendState(VK_FALSE, VK_LOGIC_OP_COPY,
                                     blendConst, colorBlendAttachState);
    tmpPipeline->initPipelineLayout("light");
    tmpPipeline->initPipeline("light");
    TKPipelineManager::manager()->allPipelines.push_back(tmpPipeline);
    TKPipelineManager::manager()->pipelineDict["light"] = tmpPipeline;
}
*/









