//
//
//
#ifndef __TKVector_h__
#define __TKVector_h__

#include "TKDataType.h"

class TKVector {
public:
    TKVector();
    ~TKVector();
    TKVector(float value, uint size);
    TKVector(uint num, ...);
    TKVector(float *arr, uint num);
    TKVector(const TKVector &v);
    
    std::string desc();
    
    float  operator [] (uint idx) const;
    float& operator [] (uint idx);
    float  dot (const TKVector &v);

    bool operator == (const TKVector &v);
    bool operator != (const TKVector &v);
    
    void operator =  (const TKVector &v);
    void operator += (const TKVector &v);
    void operator -= (const TKVector &v);
    void operator *= (const TKVector &v);
    void operator *= (const float scale);

    friend TKVector operator + (const TKVector &v0, const TKVector &v1);
    friend TKVector operator - (const TKVector &v0, const TKVector &v1);
    friend TKVector operator * (const TKVector &v0, const TKVector &v1);
    friend TKVector operator * (const TKVector &v0,  const float scale);
    friend TKVector operator * (const float scale,  const TKVector &v0);

    uint size();
    float morel();
    const float *data() const;

    void swap(TKVector &vec);
    TKVector copy();
    void copyTo(TKVector &vec);
private:
    float reserve;
    float *pData;
    uint pNumber;
};

#endif





