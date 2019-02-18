//
//TKShaderModule.h
//2018-11-26

#ifndef __TKShaderModule_h__
#define __TKShaderModule_h__

#include "../TKPrefix.h"

class TKShaderModule {
public:
    TKShaderModule();
    ~TKShaderModule();

    static TKShaderModule* createModule(const char *fileName);
    bool initWithFile(const char *fileName);

    VkShaderModule getModule() const;
    std::string getModuleName() const;
private:
    VkShaderModule m_module;
    std::string m_moduleName;
};

class TKModuleManager {
public:
    TKModuleManager();
    ~TKModuleManager();
    static TKModuleManager *shared();
    static void purge();
    static void addNewModule(const char *fileName);
    static VkShaderModule shaderModule(const char *fileName); 
    static void prepareAllModules();
private:
    std::vector<TKShaderModule *> allShaderModules;
    std::map<std::string, VkShaderModule> moduleDict;
};


#endif


