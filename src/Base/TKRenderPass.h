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
    bool initRenderPass();

    VkRenderPass renderPass() const;
    uint32_t ColorAttachCount() const;
private:
    VkRenderPass m_renderPass;
    uint32_t m_colorAttachmentCount;
};

#endif
