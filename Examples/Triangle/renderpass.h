//
//renderpass.h
//
#include "TKPrefix.h"

class RenderPass {
public:
	RenderPass();
	~RenderPass();

	static RenderPass *create();
	bool init();
	VkRenderPass renderPass();
private:
	VkRenderPass m_renderpass;
};
