//
//
//
#include "TKVector.h"
#include <stdarg.h>

TKVector::TKVector():
    pData(nullptr), pNumber(0){}

TKVector::~TKVector(){
    if (pData != nullptr){
        //  printf("delete %p\n", pData);
        free(pData);
    }
    pData = nullptr;
}

TKVector::TKVector(float value, uint size){
    pNumber = size;
    pData = (float *)malloc(sizeof(float)*size);
    //printf("stack:%p\n", pData);
    memset(pData, 0, sizeof(float)*size);
    for(int i=0; i<size; ++i){
        pData[i] = value;
    }
}

TKVector::TKVector(uint num, ...){
    pNumber = num;
    pData = (float *)malloc(sizeof(float)*num);
    //printf("stack:%p\n", pData);
    va_list va_ptr;
    va_start(va_ptr, num);
    for(int i=0; i<num; ++i){
        pData[i] = va_arg(va_ptr, double);
    }
    va_end(va_ptr);
}

TKVector::TKVector(float *arr, uint num) {
    pNumber = num;
    pData = (float *)malloc(sizeof(float)*num);
    //printf("stack:%p\n", pData);
    memcpy(pData, arr, sizeof(float)*num);
}

TKVector::TKVector(const TKVector &v){
    pNumber = v.pNumber;
    pData = (float *)malloc(sizeof(float)*pNumber);
    //printf("stack:%p\n", pData);
    memcpy(pData, v.pData, sizeof(float)*pNumber);
}

TKVector TKVector::copy(){
    return TKVector(pData, pNumber);
}

void TKVector::copyTo(TKVector &vec){
    int num = pNumber<vec.size() ? pNumber : vec.size();
    memcpy(vec.pData, pData, sizeof(float)*num);
}

std::string TKVector::desc() {
    std::string str = std::string("[");
    char buf[32];
    for(int i=0; i<pNumber-1; ++i){
        sprintf(buf, "%.3f, ", pData[i]);
        str.append(buf);
    }
    sprintf(buf, "%.3f]", pData[pNumber-1]);
    str.append(buf);
    
    return str;
}

float TKVector::operator [] (uint idx) const {
    if(idx >= pNumber){
        return 0.0f;
    }
    return pData[idx];
}

float& TKVector::operator [] (uint idx) {
    if(idx>=pNumber){
        return reserve;
    }
    return pData[idx];
}

float TKVector::dot(const TKVector &v){
    float sum = 0.0f;
    uint num = pNumber<v.pNumber ? pNumber : v.pNumber;
    for(int i=0; i<num; ++i){
        sum += pData[i]*v[i];
    }
    return sum;
}

bool TKVector::operator != (const TKVector &v){
    if(pNumber != v.pNumber){
        return true;
    }
    bool ret = true;
    for(int i=0; i<pNumber; ++i){
        if (pData[i] == v[i]){
            ret = false;
            break;
        }
    }
    return ret;
}

bool TKVector::operator == (const TKVector &v){
    if(pNumber != v.pNumber){
        return false;
    }
    return 0 == memcmp(pData, v.pData, sizeof(float)*pNumber);
}

void TKVector::operator = (const TKVector &v){
    int minNum = pNumber<v.pNumber ? pNumber : v.pNumber;
    memcpy(pData, v.pData, sizeof(float)*minNum);
}

void TKVector::operator += (const TKVector &v){
    int num = pNumber<v.pNumber ? pNumber : v.pNumber;
    for(int i=0; i<num; i+=4){
        __m128 a0 = _mm_setr_ps(pData[i%num], pData[(i+1)%num],
                                pData[(i+2)%num], pData[(i+3)%num]);
        __m128 a1 = _mm_setr_ps(v[i], v[i+1], v[i+2], v[i+3]);
        __m128 a2 = _mm_add_ps(a0, a1);
        pData[i] = a2[0];
        pData[i+1] = a2[1];
        pData[i+2] = a2[2];
        pData[i+3] = a2[3];
    }
}

void TKVector::operator -= (const TKVector &v){
    int num = pNumber<v.pNumber ? pNumber : v.pNumber;
    for(int i=0; i<num; i+=4){
        __m128 a0 = _mm_setr_ps(pData[i%num], pData[(i+1)%num],
                                pData[(i+2)%num], pData[(i+3)%num]);
        __m128 a1 = _mm_setr_ps(v[i], v[i+1], v[i+2], v[i+3]);
        __m128 a2 = _mm_sub_ps(a0, a1);
        pData[i] = a2[0];
        pData[i+1] = a2[1];
        pData[i+2] = a2[2];
        pData[i+3] = a2[3];
    }
}

void TKVector::operator *= (const TKVector &v){
    int num = pNumber<v.pNumber ? pNumber : v.pNumber;
    for(int i=0; i<num; i+=4){
        __m128 a0 = _mm_setr_ps(pData[i%num], pData[(i+1)%num],
                                pData[(i+2)%num], pData[(i+3)%num]);
        __m128 a1 = _mm_setr_ps(v[i], v[i+1], v[i+2], v[i+3]);
        __m128 a2 = _mm_mul_ps(a0, a1);
        pData[i] = a2[0];
        pData[i+1] = a2[1];
        pData[i+2] = a2[2];
        pData[i+3] = a2[3];
    }
}

void TKVector::operator *= (const float scale){
    __m128 factor = _mm_set1_ps(scale);
    for(int i=0; i<pNumber; i+=4){
        __m128 a0 = _mm_setr_ps(pData[i % pNumber], pData[(i+1) % pNumber],
                                pData[(i+2) % pNumber], pData[(i+3) % pNumber]);
        __m128 a2 = _mm_mul_ps(a0, factor);
        pData[i % pNumber] = a2[0];
        pData[(i+1) % pNumber] = a2[1];
        pData[(i+2) % pNumber] = a2[2];
        pData[(i+3) % pNumber] = a2[3];
    }
}

TKVector operator + (const TKVector &v0, const TKVector &v1) {
    uint num = v0.pNumber>v1.pNumber ? v0.pNumber : v1.pNumber;
    TKVector vec(0.0f, num);
    for(int i=0; i<num; i+=4){
        __m128 a0 = _mm_setr_ps(v0[i], v0[i+1], v0[i+2], v0[i+3]);
        __m128 a1 = _mm_setr_ps(v1[i], v1[i+1], v1[i+2], v1[i+3]);
        __m128 a2 = _mm_add_ps(a0, a1);
        vec[i+0] = a2[0];
        vec[i+1] = a2[1];
        vec[i+2] = a2[2];
        vec[i+3] = a2[3];
    }
    return vec;
}

TKVector operator - (const TKVector &v0, const TKVector &v1){
    uint num = v0.pNumber>v1.pNumber ? v0.pNumber : v1.pNumber;
    TKVector vec(0.0f, num);
    for(int i=0; i<num; i+=4){
        __m128 a0 = _mm_setr_ps(v0[i], v0[i+1], v0[i+2], v0[i+3]);
        __m128 a1 = _mm_setr_ps(v1[i], v1[i+1], v1[i+2], v1[i+3]);
        __m128 a2 = _mm_sub_ps(a0, a1);
        vec[i+0] = a2[0];
        vec[i+1] = a2[1];
        vec[i+2] = a2[2];
        vec[i+3] = a2[3];
    }
    return vec;
}

TKVector operator * (const TKVector &v0, const TKVector &v1) {
    uint num = v0.pNumber>v1.pNumber ? v0.pNumber : v1.pNumber;
    TKVector vec(0.0f, num);
    for(int i=0; i<num; i+=4){
        __m128 a0 = _mm_setr_ps(v0[i], v0[i+1], v0[i+2], v0[i+3]);
        __m128 a1 = _mm_setr_ps(v1[i], v1[i+1], v1[i+2], v1[i+3]);
        __m128 a2 = _mm_mul_ps(a0, a1);
        vec[i+0] = a2[0];
        vec[i+1] = a2[1];
        vec[i+2] = a2[2];
        vec[i+3] = a2[3];
    }
    return vec;
}

TKVector operator * (const TKVector &v, const float scale) {
    TKVector vec = v;
    vec *= scale;
    return vec;
}

TKVector operator * (const float scale, const TKVector &v) {
    TKVector vec = v;
    vec *= scale;
    return vec;
}

uint TKVector::size() { return pNumber; }

float TKVector::morel(){
    float sum = 0.0f;
    for (int i=0; i<pNumber; ++i) {
        sum += pData[i]*pData[i];
    }
    return sqrt(sum);
}

const float* TKVector::data() const{
    return pData;
}

void TKVector::swap(TKVector &vec){
    uint number = pNumber;
    float *temp = pData;
    pData = vec.pData;
    pNumber = vec.pNumber;

    vec.pData = temp;
    vec.pNumber = number;
}


