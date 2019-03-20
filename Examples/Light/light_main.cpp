//
//light_main.cpp
//2019-1-2

#include "LightScene.h"

int main(int argc, char *argv[])
{
  
    TKObjectPool::sharedPool()->startWatch();
    TKWindow *window = TKWindow::createXcbWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    window->initInstance();
    window->initSurface();
    
    TKBaseInfo::share()->initDevice();
    
    TKBaseInfo::share()->initSwapchain();
    TKBaseInfo::share()->initRenderPass();
    TKBaseInfo::share()->initFramebuffers();
    TKBaseInfo::share()->initCommandPool();
	TKBaseInfo::share()->initDescriptorPool();
    TKBaseInfo::share()->initFencesAndSemaphores();

	TKModuleManager::prepareAllModules();
    LightScene *mainScene = LightScene::createLightScene(1);
    window->setScene(mainScene);
    window->startRefresh();

	TKModuleManager::purge();
    delete window;
    
    TKObjectPool::sharedPool()->stopWatch();
    TKObjectPool::purge();
    
    TKBaseInfo::purge();
    return 0;
}
