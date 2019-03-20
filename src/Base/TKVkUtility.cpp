//
//TKVkUtility.cpp
//2019-1-18

#include "TKVkUtility.h"

static TKVkUtility *utility = nullptr;

TKVkUtility *TKVkUtility::Utility(){
    if(utility == nullptr){
        utility = new TKVkUtility;
    }
    return utility;
}

VkBool32 TKVkUtility::VkBoolFrom(const std::string &str){
    if(str.compare("VK_TRUE")==0){
        return  VK_TRUE;
    }
    return VK_FALSE;
}

VkVertexInputRate TKVkUtility::VkInputRateFrom(const std::string &str){
	int ret = strcmp(str.c_str(), "VK_VERTEX_INPUT_RATE_VERTEX");
    if(ret == 0){
		return VK_VERTEX_INPUT_RATE_VERTEX;
    }
    return VK_VERTEX_INPUT_RATE_INSTANCE;
}

VkFormat TKVkUtility::VkFormatFrom(const std::string &formatStr){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkFormat.json");
    return VkFormat(root["VkFormat"][formatStr].asUInt());
}

VkPrimitiveTopology TKVkUtility::VkPrimitiveFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return  VkPrimitiveTopology(root["PrimitiveTopology"][str].asUInt());
}

VkSharingMode TKVkUtility::VkShareModeFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return  VkSharingMode(root["SharingMode"][str].asUInt());
}

VkPolygonMode TKVkUtility::VkPolygonModeFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkPolygonMode(root["PolygonMode"][str].asUInt());
}

VkCullModeFlagBits TKVkUtility::VkCullModeFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    uint32_t value = strtoul(root["CullMode"][str].asCString(), nullptr, 16);
    return VkCullModeFlagBits(value);
}

VkFrontFace TKVkUtility::VkFrontFaceFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkFrontFace(root["FrontFace"][str].asUInt());
}

VkStencilOp TKVkUtility::VkStencilOpFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkStencilOp(root["StencilOp"][str].asUInt());
}

VkCompareOp TKVkUtility::VkCompareOpFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkCompareOp(root["CompareOp"][str].asUInt());
}

VkColorComponentFlagBits TKVkUtility::VkColorComponentFlagBitFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    uint32_t value = strtoul(root["ColorComponentFlagBit"][str].asCString(), nullptr, 16);
    return VkColorComponentFlagBits(value);
}

VkBlendFactor TKVkUtility::VkBlendFactorFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkBlendFactor(root["BlendFactor"][str].asUInt());
}

VkBlendOp TKVkUtility::VkBlendOpFrom(const std::string &str){
   Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkBlendOp(root["BlendOp"][str].asUInt());
}

VkLogicOp TKVkUtility::VkLogicOpFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkLogicOp(root["LogicOp"][str].asUInt());
}

VkSampleCountFlagBits TKVkUtility::VkSampleCountFlagBitFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    uint32_t value = strtoul(root["SampleCountFlagBit"][str].asCString(), nullptr, 16);
    return VkSampleCountFlagBits(value);
}

VkDynamicState TKVkUtility::VkDynamicStateFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkDynamicState(root["DynamicState"][str].asUInt());
}

VkDescriptorType TKVkUtility::VkDecriptorTypeFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    return VkDescriptorType(root["DescriptorType"][str].asUInt());
}

VkShaderStageFlagBits TKVkUtility::VkShaderStageFlagBitsFrom(const std::string &str){
    Json::Value root = TKVkUtility::Utility()->VkConfigFrom("vkConfig.json");
    uint32_t value = strtoul(root["ShaderStageFlagBit"][str].asCString(), nullptr, 16);
	return VkShaderStageFlagBits(value);
}

Json::Value TKVkUtility::VkConfigFrom(const std::string &fileName){
    if(m_root[fileName].isNull() == true){
        char buf0[512];
        sprintf(buf0, "%s/Json/%s", PROJECT_PATH, fileName.c_str());
        FILE *fp = fopen(buf0, "rt");
        if(fp == nullptr){
            TKLog("Open file %s failed!\n", fileName.c_str());
            assert(fp != nullptr);
            return Json::Value();
        }
        char tmp;
        uint64_t size = 1;
        while(fread(&tmp, sizeof(char), 1, fp)==1){
            ++ size;
        }
        char buf[size];
        buf[size-1] = '\0';
        fseek(fp, 0, SEEK_SET);
        fread(buf, sizeof(char), size, fp);
        std::string content(buf);
        Json::Value root;
        Json::Reader reader;
        reader.parse(content, root);
        m_root[fileName] = root;
        fclose(fp);
	}
    return m_root[fileName];
}


