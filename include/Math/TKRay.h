//
//TKRay.h
//2019-1-9

#ifndef __TKRay_h__
#define __TKRay_h__

#include "TKDataType.h"

class TKRay {
public:
    TKRay(Float3 o, Float3 d);
    ~TKRay();
    
protected:
    Float3 origin;
    Float3 direction;
    float  tMax;
};

#endif
