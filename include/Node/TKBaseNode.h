//
//TKNode.h
//2018-11-21

#ifndef __TKBaseNode_h__
#define __TKBaseNode_h__

#include "TKObject.h"
#include "TKDataDef.h"
#include "../Base/TKUniform.h"
#include "../Base/TKJsonPipeline.h"


class TKNodeList {
public:
    void *node = nullptr;
    TKNodeList *next = nullptr;
    ~TKNodeList(){
        node = nullptr;
        next = nullptr;
    }
};

class TKBaseNode: public TKObject {
public:
    TKBaseNode();
    ~TKBaseNode();
    bool initialize();
    
    void addWriteDescSet(const VkWriteDescriptorSet &writeDescSet);
    void addCopyDescSet(const VkCopyDescriptorSet &copyDescSet);
    virtual void draw(VkCommandBuffer cmdBuf, uint32_t swapIdx);
    
    void setPosition(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setRotate(Float3 axis, float angle);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    void addSubNode(TKBaseNode *node);
    void removeSubNode(TKBaseNode *node);
    void setParentNode(TKBaseNode *node);
    TKBaseNode *parentNode() const;
    TKBaseNode *subNodeWithTag(uint32_t tag);
    
    void setPipelineToUse(TKJsonPipeline *pipeline);
	
    TKMaterial frontMaterial;
    TKMaterial backMaterial;
    void refreshMaterial();

    void displayRotateMatrix() const;
    
protected:
    TKNodeList *m_subList;   
    TKBaseNode *m_parentNode;    
    
    std::vector<VkWriteDescriptorSet> m_writeDescSets;
    std::vector<VkCopyDescriptorSet>  m_copyDescSets;
    
    TKUniform     *m_basicUniform  = nullptr;
    TKUniform     *m_frontMaterialUniform = nullptr;
    TKUniform     *m_backMaterialUniform = nullptr;
    TKUniform     *m_tranUniform = nullptr;
    Float3     m_position;
    Float3     m_scale;
    TKMatrix   m_rotate, m_translation, m_scaleMat;
    TKMatrix   m_transformMat;
    
    void updateTransform();

    TKJsonPipeline *m_pipeline;
    uint32_t  m_drawIdx;
    virtual void bindUniforms(uint32_t swapIdx);    
};

#endif



