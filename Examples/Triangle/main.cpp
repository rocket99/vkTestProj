
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#include "main_scene.h"
#include "renderpass.h"


int main(int argc, char *argv[]){
	TKWindow *view = TKWindow::createXcbWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	view->initInstance();
	view->initSurface();

	TKBaseInfo::share()->initDevice();
	TKModuleManager::prepareAllModules();
	TKBaseInfo::share()->initSwapchain();

	TKBaseInfo::share()->initRenderPass("renderpass.json");
	TKBaseInfo::share()->initFramebuffers();
	TKBaseInfo::share()->initCommandPool();
	TKBaseInfo::share()->initDescriptorPool();
	TKBaseInfo::share()->initFencesAndSemaphores();
	
	MainScene *scene = MainScene::create();
	view->setScene(scene);
	view->startRefresh();
	
	TKModuleManager::purge();
	delete view;

	TKBaseInfo::purge();
	return 0;
}

