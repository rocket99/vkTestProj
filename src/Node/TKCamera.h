//
//TKCamera.h
//2018-11-11

#ifndef __TKCamera_H__
#define __TKCamera_H__

#include "../TKPrefix.h"
#include "TKMath.h"

class TKCamera {
public:
    TKCamera();
    ~TKCamera();
    static TKCamera *createCamera(Float3 pos, Float3 center, Float3 up,
                                  Float4 para = Float4(M_PI/3, 1.0, 0.01, 100.0));
    bool initCamera(Float3 pos, Float3 center, Float3 up, Float4 para);
private:
    TKMatrix *cameraMat;
    TKMatrix *perspectMat;
};


#endif
