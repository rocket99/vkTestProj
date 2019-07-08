
#include "TKEngine.h"
#include "main_scene.h"
#include "renderpass.h"

int main(int argc, char *argv[]){
	TKWindow *view = TKWindow::createXcbWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	view->initInstance();
	view->initSurface();

	TKBaseInfo::share()->initDevice();
	TKModuleManager::prepareAllModules();
	TKBaseInfo::share()->initSwapchain();
//	RenderPass *renderpass = RenderPass::create();
//	VK_INFO->renderPass = renderpass->renderPass();
//	VK_INFO->colorAttachmentCount = 1;
	TKBaseInfo::share()->initRenderPass();
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
