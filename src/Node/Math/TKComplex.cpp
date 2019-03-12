//
//
//
#include "TKComplex.h"

TKComplex::TKComplex(){
    real = 0.0f;
    imaginary = 0.0f;
    this->initSimd();
}

TKComplex::TKComplex(float a, float b){
    real = a;
    imaginary = b;
    this->initSimd();
}

TKComplex::TKComplex(const TKComplex &v){
    real = v.real;
    imaginary = v.imaginary;
    this->initSimd();
}

void TKComplex::initSimd(){
    simdData = _mm_setr_ps(real, imaginary, 0.0f, 0.0f);
}

std::string TKComplex::desc(){
    char buf[512];
    sprintf(buf, "%.2f+%.2fi", real, imaginary);
    return std::string(buf);
}

float TKComplex::morel() {
    return sqrt(real*real+imaginary*imaginary);
}

float TKComplex::argument(){
    float ll = sqrt(real*real+imaginary*imaginary);
    float arg = 0.0;
    if( imaginary>=0 ){
        arg = acos(real/ll);
    }else{
        arg = -acos(real/ll);
    }
    return arg;
}

void TKComplex::operator =  (const TKComplex &v){
    real = v.real;
    imaginary = v.imaginary;
    this->initSimd();
}

void TKComplex::operator += (const TKComplex &v){
    simdData = _mm_add_ps(simdData, v.simdData);
    real = simdData[0];
    imaginary = simdData[1];
}

void TKComplex::operator += (const float a) {
    real += a;
    this->initSimd();
}

void TKComplex::operator -= (const TKComplex &v){
    simdData = _mm_sub_ps(simdData, v.simdData);
    real = simdData[0];
    imaginary = simdData[1];
}

void TKComplex::operator -= (const float a){
    real -= a;
    this->initSimd();
}

void TKComplex::operator *= (const TKComplex &v){
    real = real*v.real - imaginary*v.imaginary;
    imaginary = real*v.imaginary + imaginary*v.real;
}

void TKComplex::operator *= (const float a){
    simdData = _mm_mul_ps(simdData, _mm_set1_ps(a));
    real = simdData[0];
    imaginary = simdData[1];
}

float TKComplex::dot(const TKComplex &v){
    return real*v.real+imaginary*v.imaginary;
}

TKComplex operator + (const TKComplex &v0, const TKComplex &v1){
    return TKComplex(v0.real+v1.real, v0.imaginary+v1.imaginary);
}

TKComplex operator - (const TKComplex &v0, const TKComplex &v1){
    return TKComplex(v0.real-v1.real, v0.imaginary-v1.imaginary); 
}

TKComplex operator * (const TKComplex &v0, const TKComplex &v1){
    return TKComplex(v0.real*v1.real - v0.imaginary*v1.imaginary,
                     v0.real*v1.imaginary + v0.imaginary*v1.real);
}

TKComplex operator * (const TKComplex &v, const float a){
    return TKComplex(v.real*a, v.imaginary*a);
}

bool TKComplex::operator == (const TKComplex &v){
    return v.real == real && imaginary == v.imaginary;
}

bool TKComplex::operator != (const TKComplex &v){
    return v.real != real || imaginary != v.imaginary;
}
