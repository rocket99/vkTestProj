//
//TKFbxNode.cpp
//2019-1-10

#include "TKFbxNode.h"

TKFbxNode::TKFbxNode(){
}

TKFbxNode::~TKFbxNode(){
}

TKFbxNode *TKFbxNode::createWithFbxFile(const string &file){
    TKFbxNode *fbxNode = new TKFbxNode;
    if(fbxNode->initWithFbxFile(file)==false){
        delete fbxNode;
        fbxNode = nullptr;
    }
    return fbxNode;
}

bool TKFbxNode::initWithFbxFile(const string &file){
    if(this->initialize()==false){
        return false;
    }

    return true;
}
