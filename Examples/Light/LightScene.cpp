//
//LightScene.cpp
//2019-1-3

#include "LightScene.h"


#define SHADER_NAME  "light"

LightScene::LightScene(){}
LightScene::~LightScene(){
    m_rootNode->release();
}

LightScene *LightScene::createLightScene(uint32_t lightNum){
    LightScene *scene = new LightScene();
    if(scene->initLightScene(lightNum)==false){
        delete scene;
        scene = nullptr;
    }
    return scene;
}

bool LightScene::initLightScene(uint32_t lightNum){
    if(this->init(SCREEN_WIDTH, SCREEN_HEIGHT)==false){
        return false;
    }

    m_rootNode = new TKBaseNode;
    if(m_rootNode->initialize() == false){
        delete m_rootNode; 
    }
    
    m_spaceSize = Float3(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
    m_camera = TKCamera::createWithLookat(Float3(0.0, 0.0, 0.0),
                                          Float3(0.0, 0.0, -500),
                                          Float3(0.0, 1.0, 0.0));
    m_camera->updateUniformData();
    m_cameraUniform = TKUniform::createWithSize(sizeof(float)*20);
    m_eyeUniform = TKUniform::createWithSize(4);
    float data[4]; memset(data, sizeof(float), 0);
    Float3 eyePos = m_camera->getPosition();
    eyePos /= Float3(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
    eyePos.copyTo(data);
    m_eyeUniform->updateData(data, sizeof(float)*3, 0);
    
    m_lightSources = new TKLightSource[1];
    Float4(0.1, 0.1, 0.1, 1.0).copyTo(m_lightSources[0].ambient);
    Float4(0.4, 0.4, 0.4, 1.0).copyTo(m_lightSources[0].diffuse);
    Float4(0.8, 0.8, 0.8, 1.0).copyTo(m_lightSources[0].specular);
    Float3(1.0, 1.0, 0.0).copyTo(m_lightSources[0].position);
    Float3(0.0, 0.0, 0.0).copyTo(m_lightSources[0].spotDirection);
    Float3(0.0, 0.0, 0.0).copyTo(m_lightSources[0].halfvector);
    m_lightSources[0].spotCosCutOff = 0.0f;
    m_lightSources[0].spotExponent = -0.6f;
    m_lightUniform = TKUniform::createWithSize(sizeof(float)*26);
    float buf[26]; m_lightSources[0].mapToBuffer(buf);
    m_lightUniform->updateData(buf, sizeof(float)*26, 0);
    
    this->initObjects();
    angle = 0.0;
    
    TKLog("init light scene ok!\n");
    return true;
}

void LightScene::initObjects(){
    TKPipeline *pipeline = TKPipelineManager::getTKPipeline(SHADER_NAME);
    TKLog("light pipeline %p\n", pipeline);

    TKGeometryObj *ground = TKGeometryObj::createRectPlane(500, 500);
    ground->setTag(1);
    ground->setPipelineToUse(pipeline);
    ground->setPosition(0.0, 0.0, 0.0);
    ground->rotateX(M_PI*0.5);
    m_rootNode->addSubNode(ground);
    ground->release();
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(ground->frontMaterial.emission);
    Float4(0.3, 0.3, 0.3, 1.0).copyTo(ground->frontMaterial.ambient);
    Float4(0.5, 0.5, 0.5, 1.0).copyTo(ground->frontMaterial.diffuse);
    Float4(1.0, 1.0, 1.0, 1.0).copyTo(ground->frontMaterial.specular);
    ground->frontMaterial.shiness = 60;
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(ground->backMaterial.emission);
    Float4(0.0, 0.5, 0.0, 1.0).copyTo(ground->backMaterial.ambient);
    Float4(0.5, 0.5, 0.5, 1.0).copyTo(ground->backMaterial.diffuse);
    Float4(1.0, 1.0, 1.0, 1.0).copyTo(ground->backMaterial.specular);
    ground->backMaterial.shiness = 20;
    ground->refreshMaterial();
    
    TKGeometryObj *sphere = TKGeometryObj::createSphere(100);
    sphere->setTag(2);
    sphere->setPipelineToUse(pipeline);
    sphere->setPosition(0.0, 100, 0.0);
    sphere->rotateX(M_PI*0.5);
    m_rootNode->addSubNode(sphere);
    sphere->displayRotateMatrix();
    sphere->release();
    Float4(0.5, 0.3, 0.0, 1.0).copyTo(sphere->frontMaterial.emission);
    Float4(1.0, 1.0, 1.0, 1.0).copyTo(sphere->frontMaterial.ambient);
    Float4(1.0, 1.0, 1.0, 1.0).copyTo(sphere->frontMaterial.diffuse);
    Float4(0.0, 1.0, 1.0, 1.0).copyTo(sphere->frontMaterial.specular);
    sphere->frontMaterial.shiness = 100;
    Float4(0.0, 0.3, 0.1, 1.0).copyTo(sphere->backMaterial.emission);
    Float4(0.2, 0.2, 0.2, 1.0).copyTo(sphere->backMaterial.ambient);
    Float4(0.5, 0.5, 0.5, 1.0).copyTo(sphere->backMaterial.diffuse);
    Float4(1.0, 0.0, 0.0, 1.0).copyTo(sphere->backMaterial.specular);
    sphere->backMaterial.shiness = 50;
    sphere->refreshMaterial();
    
    /*
    TKCylinder *cylinder = TKCylinder::createWith(100, 120);
    cylinder->setPipelineToUse(pipeline);
    cylinder->setPosition(0.0, -240.0, 0.0);
    cylinder->rotateX(M_PI*0.5);
    m_rootNode->addSubNode(cylinder);
    cylinder->release();
    Float4(0.4, 0.6, 0.8, 1.0).copyTo(cylinder->frontMaterial.emission);
    Float4(0.4, 0.3, 0.0, 1.0).copyTo(cylinder->frontMaterial.ambient);
    Float4(0.7, 0.7, 0.7, 1.0).copyTo(cylinder->frontMaterial.diffuse);
    Float4(1.0, 1.0, 1.0, 1.0).copyTo(cylinder->frontMaterial.specular);
    cylinder->frontMaterial.shiness = 0.6;
    Float4(0.2, 0.5, 0.1, 1.0).copyTo(cylinder->backMaterial.emission);
    Float4(0.0, 0.1, 0.3, 1.0).copyTo(cylinder->backMaterial.ambient);
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(cylinder->backMaterial.diffuse);
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(cylinder->backMaterial.specular);
    cylinder->backMaterial.shiness = 0.1;
    cylinder->refreshMaterial();

    TKCube *cube = TKCube::createWithSize(200, 100, 200);
    cube->setPipelineToUse(pipeline);
    cube->setPosition(0.0, -130.0, 0.0);
    m_rootNode->addSubNode(cube);
    cube->release();
    Float4(0.4, 0.6, 0.8, 1.0).copyTo(cube->frontMaterial.emission);
    Float4(0.4, 0.3, 0.0, 1.0).copyTo(cube->frontMaterial.ambient);
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(cube->frontMaterial.diffuse);
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(cube->frontMaterial.specular);
    cube->frontMaterial.shiness = 0.6;
    Float4(0.2, 0.5, 0.1, 1.0).copyTo(cube->backMaterial.emission);
    Float4(0.0, 0.1, 0.3, 1.0).copyTo(cube->backMaterial.ambient);
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(cube->backMaterial.diffuse);
    Float4(0.0, 0.0, 0.0, 1.0).copyTo(cube->backMaterial.specular);
    cube->backMaterial.shiness = 0.1;
    cube->refreshMaterial();
    */
}

void LightScene::drawObjects(){
    float data[16];
    memset(data, sizeof(float)*16, 0);
    m_spaceSize.copyTo(data);
    m_cameraUniform->updateData(data, sizeof(float)*4, 0);
    m_camera->viewProjMatrix().mapToBuffer(data);
    m_cameraUniform->updateData(data, sizeof(float)*16, 4);

    memset(data, sizeof(float)*4, 0);
    Float3 eyePos = m_camera->getPosition();
    eyePos /= Float3(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
    m_eyeUniform->updateData(data, sizeof(float)*3, 0);
    
    TKPipeline *pipeline = TKPipelineManager::getTKPipeline(SHADER_NAME);
    VkDescriptorSet descSet = pipeline->descriptorSet(m_currentIdx);
    VkCommandBuffer cmdBuf = TKBaseInfo::Info()->commandBuffers[m_currentIdx];
    vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline());
    VkWriteDescriptorSet cameraDescSet = m_cameraUniform->writeDescSet(descSet, 0);
    VkWriteDescriptorSet lightDescSet = m_lightUniform->writeDescSet(descSet, 3);
    VkWriteDescriptorSet eyeDescSet = m_eyeUniform->writeDescSet(descSet, 4);
    m_rootNode->addWriteDescSet(cameraDescSet);
    if(strcmp(SHADER_NAME, "light") == 0){
        m_rootNode->addWriteDescSet(lightDescSet);
        m_rootNode->addWriteDescSet(eyeDescSet);
    }
    m_rootNode->draw(cmdBuf, m_currentIdx);

    angle += 0.25*M_PI/180.0;
    m_camera->setPosition(600.0*cos(angle), 400.0, 600.0*sin(angle));
    m_camera->updateUniformData();
    
    TKBaseNode *node = m_rootNode->subNodeWithTag(2);
    if(node != nullptr){
        node->rotateX(0.5*M_PI/180.0);
    }
    
}


