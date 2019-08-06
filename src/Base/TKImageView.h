//
//TKImageView.h
//2018-11-12

#ifndef __TKImageView_H__
#define __TKImageView_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#include "../TKPrefix.h"

class TKImageView
{
public:
    TKImageView();
    ~TKImageView();
    static TKImageView *createDepthStencilImage(BaseVkInfo *info);
    bool initDepthStencilImage(BaseVkInfo *info);
    VkImageView imageView() const;
  
private:
    VkImage m_image;
    VkImageView m_imageView;
    VkDeviceMemory m_deviceMem; 

    uint8_t *getDataFromPng(const char *file);
    uint8_t *getDataFromJpg(const char *file);
};

#endif

