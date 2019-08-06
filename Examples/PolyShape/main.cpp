
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#include "PolyScene.h"

int main(int argc, char *aegv[]){
	TKWindow *window = TKWindow::createXcbWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	window->initInstance();
	window->initSurface();

	TKBaseInfo::share()->initDevice();
	TKModuleManager::prepareAllModules();
	TKBaseInfo::share()->initSwapchain();
	TKBaseInfo::share()->initRenderPass();
	TKBaseInfo::share()->initFramebuffers();
	TKBaseInfo::share()->initCommandPool();
	TKBaseInfo::share()->initFencesAndSemaphores();

	PolyScene *scene = PolyScene::create();
	window->setScene(scene);
	window->startRefresh();

	TKModuleManager::purge();
	delete window;

	TKBaseInfo::purge();
	return 0;
}
