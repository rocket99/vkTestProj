//
//TKShaderModule.cpp
//2018-11-26

#include "TKShaderModule.h"
#include "TKBaseInfo.h"
TKShaderModule::TKShaderModule(){
    m_module = VK_NULL_HANDLE;
}

TKShaderModule::~TKShaderModule(){
    if(m_module != VK_NULL_HANDLE){
        vkDestroyShaderModule(TKBaseInfo::Info()->device, m_module, nullptr);
    }
}

TKShaderModule *TKShaderModule::createModule(const char *fileName){
    TKShaderModule *module = new TKShaderModule();
    if (module->initWithFile(fileName) == false){
        delete module;
        module = nullptr;
    }
    return module;
}

bool TKShaderModule::initWithFile(const char *fileName){
    if(fileName==nullptr){
        return false;;
    }
    char fileStr[strlen(fileName)+10];
    sprintf(fileStr, "%s/spv/%s", PROJECT_PATH, fileName);
    
    FILE *fp = fopen(fileStr, "rb");
    if(fp == nullptr){
        TKLog("can not open file %s!\n", fileStr);
    }
    assert(fp);
    
    fseek(fp, 0, SEEK_END);
    size_t codeSize = ftell(fp);
    assert(codeSize>0 && codeSize%4 == 0);
    
    uint32_t *code = (uint32_t*)malloc(sizeof(uint32_t)*codeSize);
    fseek(fp, 0, SEEK_SET);
    fread(code, sizeof(uint32_t), codeSize, fp);
    fclose(fp);
    
    VkShaderModuleCreateInfo info;
    info.flags = 0;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.codeSize = codeSize;
    info.pCode = code;
    int ret = vkCreateShaderModule(TKBaseInfo::Info()->device,
                                   &info, nullptr, &m_module);
    free(code);
    if (ret != VK_SUCCESS){
        TKLog("create shader module failed!\n");
        return false;
    }
    m_moduleName = fileName;
    return true;
}

VkShaderModule TKShaderModule::getModule() const {
    return m_module;
}
std::string TKShaderModule::getModuleName() const {
    return m_moduleName;
}

static TKModuleManager *instance = nullptr;
TKModuleManager::TKModuleManager(){

}

TKModuleManager::~TKModuleManager(){
    moduleDict.clear();
    while(allShaderModules.size() > 0){
        delete allShaderModules[allShaderModules.size()-1];
        allShaderModules.pop_back();
    }
}

TKModuleManager* TKModuleManager::shared(){
    if(instance == nullptr){
        instance = new TKModuleManager;
    }
    return instance;
}

void TKModuleManager::purge(){
    if(instance != nullptr){
        delete instance;
    }
    instance = nullptr;
}

void TKModuleManager::addNewModule(const char *fileName){
    TKShaderModule *module = TKShaderModule::createModule(fileName);
    TKModuleManager::shared()->allShaderModules.push_back(module);
    std::string name = module->getModuleName();
    TKModuleManager::shared()->moduleDict[name] = module->getModule();
}

VkShaderModule TKModuleManager::shaderModule(const char *fileName){
    if (TKModuleManager::shared()->moduleDict[fileName] == nullptr){
		TKLog("can not find module %s\n", fileName);
        return VK_NULL_HANDLE;
    }
    return TKModuleManager::shared()->moduleDict[fileName];
}

void TKModuleManager::prepareAllModules(){
    std::vector<std::string> spvFiles = {
        "light_vert.spv", "light_frag.spv",
        "basic_vert.spv", "basic_frag.spv",
        "vert_fst.spv",   "frag_fst.spv"
    };
    
    for(int i=0; i<spvFiles.size(); ++i){
        TKModuleManager::addNewModule(spvFiles[i].c_str());
    }
}



