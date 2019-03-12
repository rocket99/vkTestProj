//
//
//
#ifndef __TKComplex_h__
#define __TKComplex_h__

#include "TKDataType.h"

class TKComplex {
public:
    ~TKComplex();
    TKComplex();
    TKComplex(float a, float b=0.0f);
    TKComplex(const TKComplex &v);

    float real;
    float imaginary;

    float morel();
    float argument();

    std::string desc();

    void operator =  (const TKComplex &v);
    void operator += (const TKComplex &v);
    void operator -= (const TKComplex &v);
    void operator += (const float a);
    void operator -= (const float a);
    void operator *= (const TKComplex &v);
    void operator *= (const float a);
    float dot(const TKComplex &v);
    
    friend TKComplex operator + (const TKComplex &v0, const TKComplex &v1);
    friend TKComplex operator - (const TKComplex &v0, const TKComplex &v1);
    friend TKComplex operator * (const TKComplex &v0, const TKComplex &v1);
    friend TKComplex operator * (const TKComplex &v,  const float a);

    bool operator == (const TKComplex &v);
    bool operator != (const TKComplex &v);
    
private:
    __m128 simdData;
    void initSimd();
};


#endif
