//
//basic_main.cpp
//2018-12-23

#include "TKEngine.h"
#include "basic_scene.h"

int main(int argc, char *argv[])
{
    TKObjectPool::sharedPool()->startWatch();
    TKWindow *window = TKWindow::createXcbWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    window->initInstance();
    window->initSurface();
    
    TKBaseInfo::share()->initDevice();
    TKModuleManager::prepareAllModules();
    TKBaseInfo::share()->initSwapchain();
    TKBaseInfo::share()->initRenderPass();
    TKBaseInfo::share()->initFramebuffers();
    TKBaseInfo::share()->initCommandPool();
    TKBaseInfo::share()->initDescriptorPool();
    TKBaseInfo::share()->initFencesAndSemaphores();

    
    TKPipelineManager::manager()->prepareAllPipelines();
    
    BasicScene *mainScene = BasicScene::createScene(SCREEN_WIDTH, SCREEN_HEIGHT);
    window->setScene(mainScene);
    window->startRefresh();

    TKPipelineManager::purge();
    TKModuleManager::purge();
    delete window;
    
    TKObjectPool::sharedPool()->stopWatch();
    TKObjectPool::purge();
    TKBaseInfo::purge();
    return 0;
}

