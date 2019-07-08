//
//
//

#include "main_scene.h"


MainScene::MainScene(){
	m_pipeline = nullptr;
}

MainScene::~MainScene(){
	if(m_pipeline != nullptr){
		delete m_pipeline;
	}
}

MainScene *MainScene::create(){
	MainScene *scene = new MainScene();
	if(scene->initScene()==false){
		delete scene;
		scene = nullptr;
	}
	return scene;
}

bool MainScene::initScene(){
	if(this->init(SCREEN_WIDTH, SCREEN_HEIGHT)==false){
		return false;
	};

	m_pipeline = TKJsonPipeline::createFromJson("first_pipeline.json");
	return true;
}


void MainScene::drawObjects(){
	VkCommandBuffer cmdBuf = TKBaseInfo::Info()->commandBuffers[m_currentIdx];
	vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->pipeline());
	vkCmdDraw(cmdBuf, 6, 1, 0, 0);
	
}


