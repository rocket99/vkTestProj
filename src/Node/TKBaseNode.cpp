//
//TKBaseNode.cpp
//2018-11-21

#include "TKBaseNode.h"
#include "TKVkBase.h"

TKBaseNode::TKBaseNode() {
    m_parentNode = nullptr;
    m_position = Float3(0.0, 0.0, 0.0);
    m_scale    = Float3(1.0, 1.0, 1.0);
    m_rotate   = TKMatrix::identity(4);
    m_translation  = TKMatrix::identity(4);
    m_scaleMat     = TKMatrix::identity(4);
    m_transformMat = TKMatrix::identity(4);
    m_pipeline     = nullptr;
}

TKBaseNode::~TKBaseNode() {
    TKLog("destruct TKBaseNode %p!\n", this);
//    VkDevice device = TKBaseInfo::Info()->device;
    delete m_basicUniform;
    delete m_tranUniform;
    delete m_frontMaterialUniform;
    delete m_backMaterialUniform;
}

bool TKBaseNode::initialize() {
    if(false == this->initObject()){
        return false;
    }
    m_position = Float3(0.0, 0.0, 0.0);
    m_scale    = Float3(1.0, 1.0, 1.0);
    m_rotate   = TKMatrix::rotation(Float3(0.0, 0.0, 1.0), 0.0);
    m_basicUniform         = TKUniform::createWithSize(32);    
    m_frontMaterialUniform = TKUniform::createWithSize(17);
    m_backMaterialUniform  = TKUniform::createWithSize(17);
    m_tranUniform          = TKUniform::createWithSize(16);
    m_subList = new TKNodeList;
    m_subList->node = this;
    m_subList->next = nullptr;
    return true;
}

void TKBaseNode::refreshMaterial() {
    float buf[17];
    memset(buf, 0, sizeof(float)*17);
    frontMaterial.mapToBuffer(&buf[0]);
    m_frontMaterialUniform->updateData(buf, sizeof(float)*17, 0);
    backMaterial.mapToBuffer(&buf[0]);
    m_backMaterialUniform->updateData(buf, sizeof(float)*17, 0);
}

void TKBaseNode::setPipelineToUse(TKJsonPipeline *pipeline) {
    m_pipeline = pipeline;
}

TKBaseNode *TKBaseNode::parentNode() const{
    return m_parentNode;
}

void TKBaseNode::setParentNode(TKBaseNode *node){
    if(node == nullptr){
        return;
    }
    if(m_parentNode != nullptr){
        TKLog("warning: set parent node failed! parent node is not nullptr!\n");
        return;
    }
    m_parentNode = node;
    this->retain();
}

void TKBaseNode::addSubNode(TKBaseNode *node) {
    bool isAdded = false;
    TKLog("%p add sub node %p\n", this, node);
    TKNodeList *lst = m_subList;
    while(lst->next != nullptr){
        if(lst->node == node){
            isAdded = true;
        }
        lst = lst->next;
    }
    if(isAdded == false){
        TKNodeList *nextList = new TKNodeList;
        lst->next = nextList;
        nextList->node = node;
        node->setParentNode(this);
    }
}

void TKBaseNode::removeSubNode(TKBaseNode *node) {
    TKNodeList *lst = m_subList;
    while(lst->next != nullptr){
        if(lst->next->node == node){
            TKNodeList *tmpLst = lst->next;
            lst->next = lst->next->next;
            delete tmpLst;
            break;
        }
        lst = lst->next;
    }
}

TKBaseNode *TKBaseNode::subNodeWithTag(uint32_t tag){
   TKNodeList *lst = m_subList;
    while(lst->next != nullptr){
        TKBaseNode *node = static_cast<TKBaseNode*>(lst->next->node);
        if(node != nullptr && node->m_tag == tag){
            return node;
        }
        lst = lst->next;
    }
    //TKLog("can't find the node with tag %u\n", tag);
    return nullptr;
}

void TKBaseNode::setPosition(float x, float y, float z) {
    m_position.x = x; m_position.y = y; m_position.z = z;
    m_translation[0][3] = m_position.x/SCREEN_WIDTH;
    m_translation[1][3] = m_position.y/SCREEN_HEIGHT;
    m_translation[2][3] = m_position.z/SCREEN_WIDTH;
    this->updateTransform();
}

void TKBaseNode::setScale(float x, float y, float z) {
    m_scale.x = x; m_scale.y = y; m_scale.z = z;
    m_scaleMat[0][0] = m_scale.x;
    m_scaleMat[1][1] = m_scale.y;
    m_scaleMat[2][2] = m_scale.z;
    this->updateTransform();
}

void TKBaseNode::rotateX(float angle) {
    m_rotate = m_rotate.cross(TKMatrix::rotation(Float3(1.0, 0.0, 0.0), angle));
    this->updateTransform();
}

void TKBaseNode::rotateY(float angle) {
    m_rotate = m_rotate.cross(TKMatrix::rotation(Float3(0.0, 1.0, 0.0), angle));
    this->updateTransform();
}

void TKBaseNode::rotateZ(float angle) {
    m_rotate = m_rotate.cross(TKMatrix::rotation(Float3(0.0, 0.0, 1.0), angle));
    this->updateTransform();
}

void TKBaseNode::setRotate(Float3 axis, float angle) {
    m_rotate = TKMatrix::rotation(axis, angle);
    this->updateTransform();
   
}

void TKBaseNode::updateTransform(){
    TKMatrix tmpMat = m_rotate.transposed().cross(m_scaleMat.transposed());
    m_transformMat = m_translation.transposed().cross(tmpMat);
    float transData[32];
    if(m_parentNode == nullptr){
        m_transformMat.mapToBuffer(&transData[0]);
        m_rotate.mapToBuffer(&transData[16]);
    }else{
        
        TKBaseNode *tmpSuperNode = m_parentNode;
        TKMatrix tmpTransMat = m_transformMat;
        while(tmpSuperNode != nullptr){
            tmpTransMat = tmpSuperNode->m_transformMat.cross(tmpTransMat);
            tmpSuperNode = tmpSuperNode->m_parentNode;
        }
        tmpTransMat.mapToBuffer(&transData[0]);
        
        TKMatrix tmpRotateMat = m_rotate;
        tmpSuperNode = m_parentNode;
        while(tmpSuperNode != nullptr){
            tmpRotateMat = tmpSuperNode->m_rotate.cross(tmpRotateMat);
            tmpSuperNode = tmpSuperNode->m_parentNode;
        }
        tmpRotateMat.mapToBuffer(&transData[16]);
    }
    m_basicUniform->updateData(transData, sizeof(float)*32, 0);
    m_tranUniform->updateData(&transData[16], sizeof(float)*16, 0);
}

void TKBaseNode::displayRotateMatrix() const {
    const TKMatrix mat = m_rotate;
    cout<<mat.desc()<<endl;
}

void TKBaseNode::draw(VkCommandBuffer cmdBuf, uint32_t swapIdx){
    TKNodeList *lst = m_subList->next;
    while(lst != nullptr){
        static_cast<TKBaseNode *>(lst->node)->draw(cmdBuf, swapIdx);
        lst = lst->next;
    }  
}

void TKBaseNode::addWriteDescSet(const VkWriteDescriptorSet &writeDescSet){
    m_writeDescSets.push_back(writeDescSet);
    TKNodeList *lst = m_subList->next;
    while(lst != nullptr){
        static_cast<TKBaseNode *>(lst->node)->addWriteDescSet(writeDescSet);
        lst = lst->next;
    }
}

void TKBaseNode::addCopyDescSet(const VkCopyDescriptorSet &copyDescSet){
    m_copyDescSets.push_back(copyDescSet);
    TKNodeList *lst = m_subList->next;
    while(lst != nullptr){
        static_cast<TKBaseNode *>(lst->node)->addCopyDescSet(copyDescSet);
        lst = lst->next;
    }
}

void TKBaseNode::bindUniforms(uint32_t swapIdx){
    if(m_pipeline != nullptr){
        VkDescriptorSet descSet = m_pipeline->descriptorSet(swapIdx);
        std::string name = m_pipeline->pipelineName();
        if(name.compare("basic") == 0){
            this->updateTransform();
            m_writeDescSets.push_back(m_basicUniform->writeDescSet(descSet, 1));
        }
        
        if(name.compare("light") == 0){
            this->updateTransform();
            m_writeDescSets.push_back(m_tranUniform->writeDescSet(descSet, 5));
            m_writeDescSets.push_back(m_basicUniform->writeDescSet(descSet, 1));
            m_writeDescSets.push_back(m_frontMaterialUniform->writeDescSet(descSet, 2, 0));
            m_writeDescSets.push_back(m_backMaterialUniform->writeDescSet(descSet, 2, 1));
        }
    }
}


