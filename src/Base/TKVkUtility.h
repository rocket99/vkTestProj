//
//TKVkUtility.h
//2019-1-18

#ifndef __TKVkUtility_h__
#define __TKVkUtility_h__

#include "../TKPrefix.h"

class TKVkUtility {
public:
    static VkBool32 VkBoolFrom(const std::string &str);
    static VkVertexInputRate VkInputRateFrom(const std::string &str);
    static VkFormat VkFormatFrom(const string &formatStr);
    static VkPrimitiveTopology VkPrimitiveFrom(const std::string &str);
    static VkSharingMode VkShareModeFrom(const std::string &str);
    static VkPolygonMode VkPolygonModeFrom(const std::string &str);
    static VkCullModeFlagBits VkCullModeFrom(const std::string &str);
    static VkFrontFace VkFrontFaceFrom(const std::string &str);
    static VkStencilOp VkStencilOpFrom(const std::string &str);
    static VkCompareOp VkCompareOpFrom(const std::string &str);
    static VkLogicOp VkLogicOpFrom(const std::string &str);
    static VkColorComponentFlagBits VkColorComponentFlagBitFrom(const std::string &str);
    static VkBlendFactor VkBlendFactorFrom(const std::string &str);
    static VkBlendOp VkBlendOpFrom(const std::string &str);
    static VkSampleCountFlagBits VkSampleCountFlagBitFrom(const std::string &str);
    static VkDynamicState VkDynamicStateFrom(const std::string &str);
    static VkDescriptorType VkDecriptorTypeFrom(const std::string &str);
	static VkShaderStageFlagBits VkShaderStageFlagBitsFrom(const std::string &str);

	static VkAttachmentLoadOp VkAttachmentLoadOpFromString(const std::string &str);
	static VkAttachmentStoreOp VkAttachmentStoreOpFromString(const std::string &str);
	static VkPipelineStageFlagBits VkPipelineStageFlagBitFrom(const std::string &str);
	static VkAccessFlagBits VkAccessFlagBitFrom(const std::string &str);
	static VkImageLayout VkImageLayoutFromString(const std::string &str);
	static VkPipelineBindPoint VkPipelineBindPointFromString(const std::string &str);

	
    Json::Value VkConfigFrom(const std::string &fileName);
    static TKVkUtility *Utility();
private:
    Json::Value m_root;
};
#endif


