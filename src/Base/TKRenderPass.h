//
//TKRenderPass.h
//2018-11-17

#ifndef __TKRenderPass_h__
#define __TKRenderPass_h__

#include "../TKPrefix.h"

class TKRenderPass {
public:
    TKRenderPass();
    ~TKRenderPass();
    static TKRenderPass *createRenderPass();
	static TKRenderPass *createWithJson(const std::string &fileName);
	bool initWithJson(const std::string &fileName);

    VkRenderPass renderPass() const;
    uint32_t ColorAttachCount() const;
private:
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    uint32_t m_colorAttachmentCount;

	VkSubpassDependency _getDependencyFromJson(const Json::Value &value);
	VkAttachmentReference _getAttachmentRefFromJson(const Json::Value &value);
	VkSubpassDescription _getSubpassDescFromJson(const Json::Value &value, uint32_t idx=0);
	VkAttachmentDescription _getAttachmentDescription(const Json::Value &value);
	
	std::map< std::string, std::vector<VkAttachmentReference> > m_allAttachmentRefInfo;
	std::map< std::string, std::vector<uint32_t> > m_IntAttachInfo;
};

#endif
