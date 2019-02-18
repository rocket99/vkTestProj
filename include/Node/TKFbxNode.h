//
//TKFbxNode.h
//2019-1-10

#ifndef __TKFbxNode_h__
#define __TKFbxNode_h__

#include "TKBaseNode.h"

class TKFbxNode : public TKBaseNode
{
public:
    TKFbxNode();
    ~TKFbxNode();
    static TKFbxNode *createWithFbxFile(const string &file);
    bool initWithFbxFile(const string &file);
protected:
    
};

#endif
