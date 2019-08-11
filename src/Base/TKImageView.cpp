//
//TKImageview.cpp
//2018-11-12

#include "TKImageView.h"
#include "TKBaseInfo.h"
#include "TKWindow.h"

TKImageView::TKImageView() {
    m_image     = VK_NULL_HANDLE;
    m_imageView = VK_NULL_HANDLE;
    m_deviceMem = VK_NULL_HANDLE;
}

TKImageView::~TKImageView() {
    vkFreeMemory(VK_INFO->device, m_deviceMem, nullptr);
    vkDestroyImage(VK_INFO->device, m_image, nullptr);
    vkDestroyImageView(VK_INFO->device, m_imageView, nullptr);
}

VkImageView TKImageView::imageView() const {
    return m_imageView;
}

TKImageView *TKImageView::createDepthStencilImage(BaseVkInfo *info) {
    TKImageView *imgView = new TKImageView();
    if(imgView->initDepthStencilImage(info) == false){
        delete imgView;
        imgView = nullptr;
    }
    return imgView;
}

bool TKImageView::initDepthStencilImage(BaseVkInfo *info) {
    VkFormatProperties property;
    vkGetPhysicalDeviceFormatProperties(info->physicalDevice,
                                        VK_FORMAT_D32_SFLOAT, &property);
    /*
    TKLog("linear tiling features 0x%08X\n", property.linearTilingFeatures);
    TKLog("optimal tiling features 0x%08X\n", property.optimalTilingFeatures);
    TKLog("buffer features 0x%08X\n", property.bufferFeatures);
    */
    VkImageFormatProperties formatProperty;
    VkResult ret = vkGetPhysicalDeviceImageFormatProperties(info->physicalDevice, VK_FORMAT_D32_SFLOAT,
                                                            VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL,
                                                            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                                            VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
                                                            &formatProperty);
    /*
    TKLog("max extent3D {%d, %d, %d}\n", formatProperty.maxExtent.width,
           formatProperty.maxExtent.height, formatProperty.maxExtent.depth);
    TKLog("max mip level %d\n", formatProperty.maxMipLevels);
    TKLog("max array layers %d\n", formatProperty.maxArrayLayers);
    TKLog("sample count %d\n", formatProperty.sampleCounts);
    TKLog("max resource size %lu\n", formatProperty.maxResourceSize);
    */
    
    VkImageCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.imageType    = VK_IMAGE_TYPE_2D;
    createInfo.format       = VK_FORMAT_D32_SFLOAT;
    createInfo.extent       = {SCREEN_WIDTH, SCREEN_HEIGHT, 1};
    createInfo.mipLevels    = 1;
    createInfo.arrayLayers  = 1;
    createInfo.samples      = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling       = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage        = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    createInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices   = nullptr;
    createInfo.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
    ret = vkCreateImage(info->device, &createInfo, nullptr, &m_image);
    if (ret != VK_SUCCESS) {
        printf("create depth stencil image failed!\n");
        return false;
    }
    VkMemoryRequirements requirement;
    vkGetImageMemoryRequirements(info->device, m_image, &requirement);
    
    VkPhysicalDeviceMemoryProperties memoryProperty;
    vkGetPhysicalDeviceMemoryProperties(info->physicalDevice, &memoryProperty);
    uint32_t memoryTypeIdx = 0;
    VkMemoryPropertyFlags requiredMemoryFlag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    for (uint32_t i=0; i<memoryProperty.memoryTypeCount; ++i){
        if(requirement.memoryTypeBits & (1 << i)){
            if(requiredMemoryFlag == (memoryProperty.memoryTypes[i].propertyFlags & requiredMemoryFlag)){
                memoryTypeIdx = i;
                break;
            }
        }
    }
    TKLog("memory type index %d\n", memoryTypeIdx);
    
    VkMemoryAllocateInfo memoryAllocInfo;
    memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocInfo.pNext = nullptr;
    memoryAllocInfo.allocationSize = requirement.size;
    memoryAllocInfo.memoryTypeIndex = memoryTypeIdx;
    ret = vkAllocateMemory(info->device, &memoryAllocInfo, nullptr, &m_deviceMem);
    if (ret != VK_SUCCESS) {
        printf("create depth image memory failed!\n");
        return false;
    }
    
    ret = vkBindImageMemory(info->device, m_image, m_deviceMem, 0);
    if (ret != VK_SUCCESS) {
        TKLog("bind memory to depth Image failed!\n");
        return false;
    }
    TKLog("depth image bind memory!\n");
    VkImageViewCreateInfo viewInfo;
    viewInfo.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.pNext        = nullptr;
    viewInfo.flags        = 0;
    viewInfo.image        = m_image;
    viewInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format       = VK_FORMAT_D32_SFLOAT;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;
	if (VK_FORMAT_D32_SFLOAT >= VK_FORMAT_D16_UNORM_S8_UINT){
		viewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
    ret = vkCreateImageView(info->device, &viewInfo, nullptr, &m_imageView);
    if(ret != VK_SUCCESS) {
        TKLog("create depth image view failed!\n");
        return false;
    }
    TKLog("step 5: depth stencil image view was created success!\n");
    return true;    
}

uint8_t *TKImageView::getDataFromJpg(const char *file){
    FILE *fp = fopen(file, "rb");
    if(fp == nullptr){
        TKLog("can't open file %s\n", file);
        return nullptr;
    }
    uint8_t *tmpData = nullptr;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr  jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, true);
    uint32_t width = cinfo.image_width;
    uint32_t height = cinfo.image_height;
    tmpData = (uint8_t*)malloc(sizeof(uint8_t)*width*height*cinfo.num_components);
    jpeg_start_decompress(&cinfo);
    JSAMPROW row_pointer[1];
    while(cinfo.output_scanline > cinfo.output_height){
        row_pointer[0] = &tmpData[cinfo.output_scanline*cinfo.image_width*cinfo.num_components];
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t)*width*height*4);
    for(uint32_t y=0; y<height; ++y){
        for(uint32_t x=0; x<width; ++x){
            data[4*(y*width+x)+0] = tmpData[3*(y*width+x)+0];
            data[4*(y*width+x)+1] = tmpData[3*(y*width+x)+1];
            data[4*(y*width+x)+2] = tmpData[3*(y*width+x)+2];
            data[4*(y*width+x)+3] = UINT8_MAX;
        }
    }
    free(tmpData);
    
    return data;
}

uint8_t *TKImageView::getDataFromPng(const char *file){
    FILE *fp = fopen(file, "rb");
    if(fp == nullptr){
        TKLog("can't open file %s\n", file);
        return nullptr;
    }
    uint8_t *data = nullptr;

    png_structp png_ptr;
    png_infop  info_ptr;
    png_bytep *row_ptrs;
    uint8_t buf[PNG_BYTES_TO_CHECK];
    uint32_t temp, color_type;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    info_ptr = png_create_info_struct(png_ptr);
    setjmp(png_jmpbuf(png_ptr));
    temp = fread(buf, sizeof(uint8_t), PNG_BYTES_TO_CHECK, fp);
    assert(temp >= PNG_BYTES_TO_CHECK);
    if(temp < PNG_BYTES_TO_CHECK){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return nullptr;
    }
    temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
    if(temp != 0){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return nullptr;
    }
    rewind(fp);
    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
    color_type = png_get_color_type(png_ptr, info_ptr);
    uint32_t width = png_get_image_width(png_ptr, info_ptr);
    uint32_t height = png_get_image_height(png_ptr, info_ptr);
    row_ptrs = png_get_rows(png_ptr, info_ptr);
    data = (uint8_t*)malloc(sizeof(uint8_t)*width*height*4);
    uint64_t oft = 0;
    switch(color_type){
    case PNG_COLOR_TYPE_RGB_ALPHA:
        for(uint32_t y = 0; y < height; ++ y){
            for(uint32_t x = 0; x < 4*width; x += 4){
                data[oft++] = row_ptrs[y][x+0];
                data[oft++] = row_ptrs[y][x+1];
                data[oft++] = row_ptrs[y][x+2];
                data[oft++] = row_ptrs[y][x+3];
            }
        }
        break;
    case PNG_COLOR_TYPE_RGB:
        for(uint32_t y = 0; y < height; ++ y){
            for(uint32_t x = 0; x < 3*width; x += 3){
                data[oft++] = row_ptrs[y][x+0];
                data[oft++] = row_ptrs[y][x+1];
                data[oft++] = row_ptrs[y][x+2];
                data[oft++] = 255;
            }
        }        
        break;
 
    default:
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        fclose(fp);
        break;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return data;
}




