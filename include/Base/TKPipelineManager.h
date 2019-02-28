//
//TKPipelineManager.h
//2018-11-27

#ifndef __TKPipelineManager_h__
#define __TKPipelineManager_h__

#include "TKPipeline.h"
#include "TKJsonPipeline.h"

class TKPipelineManager {
public:
    TKPipelineManager();
    ~TKPipelineManager();
    static TKPipelineManager *manager();
    static void purge();
    static VkPipeline getPipeline(const std::string &pipelineName);
    static TKJsonPipeline *getTKJsonPipeline(const std::string &pipelineName); 
    static void addNewPipeline(TKJsonPipeline *pipeline, std::string name);
    void prepareAllPipelines();
private:
    //std::map<std::string, TKPipeline*> pipelineDict;
	std::map<std::string, TKJsonPipeline*>pipelineMap;
    std::vector<TKJsonPipeline *> allPipelines;
    void preparePipelineFromJson(const char *fileName);
    
    void prepareBasicPipeline();
    void prepareFirstPipeline();
    void prepareLightPipeline();
};

#endif


