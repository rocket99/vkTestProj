//
//
#ifndef __TKDataType_h__
#define __TKDataType_h__

#include <immintrin.h>
#include <smmintrin.h>
#include <nmmintrin.h>
#include <tmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <wmmintrin.h>
#include <mmintrin.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <functional>
using namespace std;

#define SIMD_ENABLED  1

struct Int2 {
public:
    int& x = data[0];
    int& y = data[1];
    Int2(int a, int b);
    Int2(const Int2 &value);

    void operator  = (const Int2 &value);
    void operator += (const Int2 &value);
    void operator -= (const Int2 &value);
    void operator *= (const Int2 &value);
    void operator *= (const int factor);
    void operator /= (const Int2 &value);
    void operator /= (const int factor);
    bool operator == (const Int2 &value);

    friend Int2 operator + (const Int2 &v0, const Int2 &v1);
    friend Int2 operator - (const Int2 &v0, const Int2 &v1);
    friend Int2 operator * (const Int2 &v0, const Int2 &v1);
    friend Int2 operator * (const Int2 &v, const int factor);
    friend Int2 operator * (const int factor, const Int2 &v);
    friend Int2 operator / (const Int2 &v0, const Int2 &v1);
    friend Int2 operator / (const Int2 &v, const int factor);

    int operator [] (uint32_t idx) const;
    int& operator [] (uint32_t idx);
    std::string desc();

    void copyTo(int *dstData);
private:
    int data[2];
    __m128i initSimd();
    __m128i initSimd() const;
};

struct Int3 {
    int& x = data[0];
    int& y = data[1];
    int& z = data[2];
    
    Int3(){x=0; y=0; z=0;}
    Int3(int a, int b, int c);
    Int3(const Int3 &v);
    void operator  = (const Int3 &v);
    void operator += (const Int3 &v);
    void operator -= (const Int3 &v);
    void operator *= (const Int3 &v);
    void operator *= (const int factor);
    void operator /= (const Int3 &v);
    void operator /= (const int factor);
    bool operator == (const Int3 &v);

    friend Int3 operator + (const Int3 &v0, const Int3 &v1);
    friend Int3 operator - (const Int3 &v0, const Int3 &v1);
    friend Int3 operator * (const Int3 &v0, const Int3 &v1);
    friend Int3 operator * (const Int3 &v, const int factor);
    friend Int3 operator * (const int factor, const Int3 &v);
    friend Int3 operator / (const Int3 &v0, const Int3 &v1);
    friend Int3 operator / (const Int3 &v, const int factor);
    void copyTo(int *dstData);
private:
    int data[3];
    __m128i initSimd();
    __m128i initSimd() const;
};
    
struct Int4 {
public:
    int &x = data[0]; int &y = data[1];
    int &z = data[2]; int &w = data[3];
    Int4(){x=0; y=0; z=0; w=0;};
    Int4(int a, int b, int c, int d);

    bool operator == (const Int4 &v);
    void operator =  (const Int4 &v);
    void operator += (const Int4 &v);
    void operator -= (const Int4 &v);
    void operator *= (const Int4 &v);
    void operator *= (const int factor);
    void operator /= (const Int4 &v);
    void operator /= (const int factor);

    friend Int4 operator + (const Int4 &v0, const Int4 &v1);
    friend Int4 operator - (const Int4 &v0, const Int4 &v1);
    friend Int4 operator * (const Int4 &v0, const Int4 &v1);
    friend Int4 operator * (const Int4 &v, const int factor);
    friend Int4 operator * (const int factor, const Int4 &v);
    friend Int4 operator / (const Int4 &v0, const Int4 &v1);
    friend Int4 operator / (const Int4 &v, const int factor);
    std::string desc();
    void copyTo(int *dstData);
    
private:
    int data[4];
    __m128i initSimd();
    __m128i initSimd() const;
};

struct Float2 {
public:
    float &x = data[0];
    float &y = data[1];
    Float2(){x=0.0; y=0.0;}
    Float2(float a, float b);
    Float2(const Float2 &v);

    Float2 normal() const;
    void normalized();
    float dot(const Float2 &v);
    
    bool operator == (const Float2 &v);
    void operator =  (const Float2 &v);
    void operator += (const Float2 &v);
    void operator -= (const Float2 &v);
    void operator *= (const Float2 &v);
    void operator *= (const float factor);
    void operator /= (const Float2 &v);
    void operator /= (const float factor);

    friend Float2 operator + (const Float2 &v0, const Float2 &v1);
    friend Float2 operator - (const Float2 &v0, const Float2 &v1);
    friend Float2 operator * (const Float2 &v0, const Float2 &v1);
    friend Float2 operator * (const float factor, const Float2 &v);
    friend Float2 operator * (const Float2 &v, const float factor);
    friend Float2 operator / (const Float2 &v0, const Float2 &v1);
    friend Float2 operator / (const Float2 &v, const float factor);

    void copyTo(float *dstData);
    std::string desc();
private:
    float data[2];
    __m128 initSimd();
    __m128 initSimd() const;
};

struct Float3 {
public:
    float &x = data[0]; float &y = data[1];
    float &z = data[2];
    Float3() {x=0.0f; y=0.0f; z=0.0f;}
    Float3(float a, float b, float c);
    Float3(const Float3 &v);

    Float3 normal() const;
    void normalized();
    Float3 cross(const Float3 &value);
    float dot(const Float3 &v);
    bool operator == (const Float3 &v);
    void operator =  (const Float3 &v);
    void operator += (const Float3 &v);
    void operator -= (const Float3 &v);
    void operator *= (const Float3 &v);
    void operator *= (const float factor);
    void operator /= (const Float3 &v);
    void operator /= (const float factor);

    friend Float3 operator + (const Float3 &v0, const Float3 &v1);
    friend Float3 operator - (const Float3 &v0, const Float3 &v1);
    friend Float3 operator * (const Float3 &v0, const Float3 &v1);
    friend Float3 operator * (const float factor, const Float3 &v);
    friend Float3 operator * (const Float3 &v, const float factor);
    friend Float3 operator / (const Float3 &v0, const Float3 &v1);
    friend Float3 operator / (const Float3 &v, const float factor);

    void copyTo(float *dstData);
    std::string desc();
private:
    float data[3];
    __m128 initSimd();
    __m128 initSimd() const;
};

struct Float4 {
    float &x = data[0]; float &y = data[1];
    float &z = data[2]; float &w = data[3];

    Float4(){
        memset(data, 0, sizeof(float)*4);
    }

    Float4(float a, float b, float c, float d);
    Float4(const Float4 &v);
    Float4 normal() const;
    void normalized();
    float dot(const Float4 &v);
    bool operator == (const Float4 &v);
    
    void operator =  (const Float4 &v);
    void operator += (const Float4 &v);
    void operator -= (const Float4 &v);
    void operator *= (const Float4 &v);
    void operator *= (const float factor);
    void operator /= (const Float4 &v);
    void operator /= (const float factor);

    friend Float4 operator + (const Float4 &v0, const Float4 &v1);
    friend Float4 operator - (const Float4 &v0, const Float4 &v1);
    friend Float4 operator * (const Float4 &v0, const Float4 &v1);
    friend Float4 operator * (const float factor, const Float4 &v);
    friend Float4 operator * (const Float4 &v, const float factor);
    friend Float4 operator / (const Float4 &v0, const Float4 &v1);
    friend Float4 operator / (const Float4 &v, const float factor);

    void copyTo(float *dstData);
    std::string desc();
private:
    float data[4];
    __m128 initSimd();
    __m128 initSimd() const;
};

struct Double2 {
    double &x = data[0];
    double &y = data[1];
    Double2(){
        memset(data, 0, sizeof(double)*2);
    }
    
    Double2(double a, double b);
    Double2(const Double2 &v);
    
    Double2 normal() const;
    void normalized();
    double dot(const Double2 &v);

    bool operator == (const Double2 &v);
    void operator =  (const Double2 &v);
    void operator += (const Double2 &v);
    void operator -= (const Double2 &v);
    void operator *= (const Double2 &v);
    void operator *= (const double factor);
    void operator /= (const Double2 &v);
    void operator /= (const double factor);

    friend Double2 operator + (const Double2 &v0, const Double2 &v1);
    friend Double2 operator - (const Double2 &v0, const Double2 &v1);
    friend Double2 operator * (const Double2 &v0, const Double2 &v1);
    friend Double2 operator * (const Double2 &v, const double factor);
    friend Double2 operator * (const double factor, const Double2 &v);
    friend Double2 operator / (const Double2 &v0, const Double2 &v1);
    friend Double2 operator / (const Double2 &v, const double factor);
    void copyTo(double *dstData);
    std::string desc();
private:
    double data[2];
    __m128d initSimd();
    __m128d initSimd() const;
};

struct Double3 {
    double &x = data[0]; double &y = data[1];
    double &z = data[2];
    Double3(){x=0.0; y=0.0; z=0.0;}
    Double3(double a, double b, double c);
    Double3(const Double3 &v);

    Double3 normal() const;
    void normalized();
    Double3 cross(const Double3 &value);
    double dot(const Double3 &v);
    
    bool operator == (const Double3 &v);
    void operator =  (const Double3 &v);
    void operator += (const Double3 &v);
    void operator -= (const Double3 &v);
    void operator *= (const Double3 &v);
    void operator *= (const double factor);
    void operator /= (const Double3 &v);
    void operator /= (const double factor);

    friend Double3 operator + (const Double3 &v0, const Double3 &v1);
    friend Double3 operator - (const Double3 &v0, const Double3 &v1);
    friend Double3 operator * (const Double3 &v0, const Double3 &v1);
    friend Double3 operator * (const Double3 &v, const double factor);
    friend Double3 operator * (const double factor, const Double3 &v);
    friend Double3 operator / (const Double3 &v0, const Double3 &v1);
    friend Double3 operator / (const Double3 &v, const double factor);

    void copyTo(double *dstData);
    std::string desc();
private:
    double data[3];
    __m256d initSimd();
    __m256d initSimd() const;
};

struct Double4 {
    double &x = data[0]; double &y = data[1];
    double &z = data[2]; double &w = data[3];
    
    Double4(){x=0.0; y=0.0; z=0.0; w=0.0;}
    Double4(double a, double b, double c, double d);
    Double4(const Double4 &v);

    Double4 normal() const;
    void normalized();
    double dot(const Double4 &v);
    
    bool operator == (const Double4 &v);
    void operator =  (const Double4 &v);
    void operator += (const Double4 &v);
    void operator -= (const Double4 &v);
    void operator *= (const Double4 &v);
    void operator *= (const double factor);
    void operator /= (const Double4 &v);
    void operator /= (const double factor);

    friend Double4 operator + (const Double4 &v0, const Double4 &v1);
    friend Double4 operator - (const Double4 &v0, const Double4 &v1);
    friend Double4 operator * (const Double4 &v0, const Double4 &v1);
    friend Double4 operator * (const Double4 &v, const double factor);
    friend Double4 operator * (const double factor, const Double4 &v);
    friend Double4 operator / (const Double4 &v0, const Double4 &v1);
    friend Double4 operator / (const Double4 &v, const double factor);

    void copyTo(double *dstData);
    std::string desc();
private:
    double data[4];
    __m256d initSimd();
    __m256d initSimd() const;
};

#endif


