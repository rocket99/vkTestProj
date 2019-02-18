//
//basic_scene.cpp
//2018-12-23

#include "basic_scene.h"

BasicScene::BasicScene(){
    m_camera = nullptr;
}

BasicScene::~BasicScene(){
    if(m_camera != nullptr){
        delete m_camera;
    }
}

BasicScene *BasicScene::createScene(uint32_t width, uint32_t height){
    BasicScene *scene = new BasicScene();
    if(scene->initWithSize(width, height) == false){
        delete scene;
        scene = nullptr;
    }
    return scene;
}

bool BasicScene::initWithSize(uint32_t width, uint32_t height){
    this->init(width, height);

    /*
    VkDebugReportCallbackEXT debugCallback;
    VkDebugReportCallbackCreateInfoEXT debugInfo;
    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debugInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
    debugInfo.pNext = nullptr;
    debugInfo.pUserData = nullptr;
    debugInfo.pfnCallback = nullptr;
    vkCreateDebugReportCallbackEXT(TKBaseInfo::Info()->instance,
                                   &debugInfo, nullptr, &debugCallback);
    vkDebugReportMessageEXT(TKBaseInfo::Info()->instance,
                            VK_DEBUG_REPORT_INFORMATION_BIT_EXT,
                            VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT,
                            VK_NULL_HANDLE, 0, 0, nullptr, nullptr);
    */
    
    m_camera = TKCamera::createWithLookat(Float3(0.0, -100.0, 0.0),
                                          Float3(0, 0.0, 2000.0),
                                          Float3(0.0, 1.0, 0.0));
    m_uniform = TKUniform::createWithSize(20);
    float data[16]; memset(data, sizeof(float)*16, 0);
    m_spaceSize = Float3(width, height, width);
    m_spaceSize.copyTo(data);
    m_uniform->updateData(data, sizeof(float)*4, 0);
    m_camera->viewProjMatrix().mapToBuffer(data);
    m_uniform->updateData(data, sizeof(float)*16, 4);
    TKPipeline *pipeline = TKPipelineManager::manager()->getTKPipeline("basic");
    TKLog("basic scene init ok!\n");
    return true;
}

void BasicScene::drawObjects(){
    theta += 0.2/180.0*M_PI;
    m_camera->setPosition(2000*cos(theta), 100.0, 2000*sin(theta));
    m_camera->updateUniformData();

    float data[16];
    memset(data, sizeof(float)*16, 0);
    m_spaceSize.copyTo(data);
    m_uniform->updateData(data, sizeof(float)*4, 0);
    m_camera->viewProjMatrix().mapToBuffer(data);
    m_uniform->updateData(data, sizeof(float)*16, 4);
    
    TKPipeline *pipeline = TKPipelineManager::manager()->getTKPipeline("basic");
    VkDescriptorSet descSet = pipeline->descriptorSet(m_currentIdx);
    VkCommandBuffer cmdBuf = TKBaseInfo::Info()->commandBuffers[m_currentIdx];
    vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline());

    VkWriteDescriptorSet writeDescSet = m_uniform->writeDescSet(descSet, 0);
}

