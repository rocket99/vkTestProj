//
//TKCamera.h
//2018-12-27

#ifndef __TKCamera_h__
#define __TKCamera_h__

#include "TKPrefix.h"
#include "TKDataType.h"
#include "TKVector.h"
#include "TKMatrix.h"

class TKCamera final {
public:
    TKCamera();
    ~TKCamera();
    static TKCamera * createWithLookat(const Float3 &center, const Float3 &pos, const Float3 &up);
    bool initWithLookAtPara(const Float3 &center, const Float3 &pos, const Float3 &up);
    void updateUniformData();
    Float3 getPosition() const;
    Float3 getPointCenter() const;
    void setPosition(float x, float y, float z);
    void roll(float angle);
    void pitch(float angle);
    void yaw(float angle);
    const TKMatrix &viewProjMatrix() const;
private:
    Float3 m_pos, m_center, m_up;
    Float3 nx, ny, nz;
    float  m_aspect, m_fovy, m_zNear, m_zFar;
    TKMatrix m_projMat;
    TKMatrix m_lookatMat;
    TKMatrix m_viewProjMat;
    void normalized();
};

#endif
