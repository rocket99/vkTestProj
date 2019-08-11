//
//
#include"TKDataType.h"

Int2::Int2(int a, int b) {
    x = 0;
    y = 0;
}

Int2::Int2(const Int2 &value) {
    x = value.x;
    y = value.y;
}

int Int2::operator [] (uint32_t idx) const {
    printf("const int\n");
    int result;
    switch(idx){
    case 0:
        result = x;
        break;
    case 1:
        result = y;
        break;
    default:
        throw std::string("index out of range");
        break;
    }
    return result;
}

int & Int2::operator[] (uint32_t idx){
    printf("refrerence int\n");
    switch(idx){
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    default:
        throw "index out of range";
        break;
    }
    return x;
}

std::string Int2::desc(){
    char buf[256];
    sprintf(buf, "[%d, %d]", x, y);
    return std::string(buf);
}

inline __m128i Int2::initSimd() {
    return _mm_setr_epi32(x, x, y, y);
}

inline __m128i Int2::initSimd() const {
    return _mm_setr_epi32(x, x, y, y);
}

void Int2::operator = (const Int2 &value) {
    x = value.x;
    y = value.y;
}

void Int2::operator += (const Int2 &value) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = value.initSimd();
        __m128i data = _mm_add_epi32(data0, data1);
        x = _mm_extract_epi32(data, 0);
        y = _mm_extract_epi32(data, 2);
        return;
    }
    x += value.x;
    y += value.y;
}

void Int2::operator -= (const Int2 &value) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = value.initSimd();
        __m128i data = _mm_sub_epi32(data0, data1);
        x = _mm_extract_epi32(data, 0);
        y = _mm_extract_epi32(data, 2);
        return;
    }
    x -= value.x;
    y -= value.y;
}

void Int2::operator *= (const Int2 &value) {
    if(SIMD_ENABLED == 1){    
        __m128i data0 = _mm_setr_epi32(x, x, y, y);
        __m128i data1 = _mm_setr_epi32(value.x, value.x, value.y, value.y);
        __m128i ret0 = _mm_mul_epi32(data0, data1);
//        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        x = _mm_extract_epi32(ret0, 0);
        y = _mm_extract_epi32(ret0, 2);
        return;
    }
    x*=value.x;
    y*=value.y;
}

void Int2::operator *= (const int factor) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = _mm_set1_epi32(factor);
        __m128i ret = _mm_mul_epi32(data0, data1);
        x = _mm_extract_epi32(ret, 0);
        y = _mm_extract_epi32(ret, 2);
        return;
    }
    x *= factor;
    y *= factor;
}

void Int2::operator /= (const Int2 &value) {
    if(SIMD_ENABLED == 1){
        x /= value.x;
        y /= value.y;
        return;
    }
    x /= value.x;
    y /= value.y;
}

void Int2::operator /= (const int factor) {
    if(SIMD_ENABLED == 1){
        x /= factor;
        y /= factor;
        return;
    }
    x /= factor;
    y /= factor;
}

bool Int2::operator == (const Int2 &value) {
    return x==value.x && y==value.y;
}

Int2 operator + (const Int2 &v0, const Int2 &v1) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret = _mm_add_epi32(data0, data1);
        return Int2(_mm_extract_epi32(ret, 0), _mm_extract_epi32(ret, 1));
    }
    return Int2(v0.x+v1.x, v0.y*v1.y);
}

Int2 operator - (const Int2 &v0, const Int2 &v1) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret = _mm_sub_epi32(data0, data1);
        return Int2(_mm_extract_epi32(ret, 0), _mm_extract_epi32(ret, 2));
    }
    return Int2(v0.x-v1.x, v0.y-v1.y);
}

Int2 operator * (const Int2 &v0, const Int2 &v1) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = _mm_setr_epi32(v0.x, v0.x, v0.y, v0.y);
        __m128i data1 = _mm_setr_epi32(v1.x, v1.x, v1.y, v1.y);
        __m128i ret0 = _mm_mul_epi32(data0, data1);
//        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        return Int2(_mm_extract_epi32(ret0, 0), _mm_extract_epi32(ret0, 2));
    }
    return Int2(v0.x*v1.x, v0.y*v1.y);
}

Int2 operator * (const Int2 &v, const int factor) {
    return v*Int2(factor, factor);
}

Int2 operator * (const int factor, const Int2 &v) {
    return Int2(factor, factor)*v;
}

Int2 operator / (const Int2 &v0, const Int2 &v1) {
    if(SIMD_ENABLED == 1){
        return Int2(v0.x/v1.x, v0.y/v1.y);
    }
    return Int2(v0.x/v1.x, v0.y/v1.y);
}

void Int2::copyTo(int *dstData){
    memcpy(dstData, data, sizeof(int)*2);
}

#pragma mark Int3
Int3::Int3(int a, int b, int c) {
    x=a; y=b; z=c;
}

Int3::Int3(const Int3 &v) {
    x=v.x; y=v.y; z=v.z;
}

inline __m128i Int3::initSimd() {
    return _mm_setr_epi32(x, y, z, 0);
}

inline __m128i Int3::initSimd() const {
    return _mm_setr_epi32(x, y, z, 0);
}

void Int3::operator = (const Int3 &v) {
    x=v.x; y=v.y; z=v.z;
}

void Int3::operator += (const Int3 &v) {
    if (SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = v.initSimd();
        __m128i ret = _mm_add_epi32(data0, data1);
        x = _mm_extract_epi32(ret, 0);
        y = _mm_extract_epi32(ret, 1);
        z = _mm_extract_epi32(ret, 2);
        return;
    }
    x += v.x; y += v.y; z += v.z;
}

void Int3::operator -= (const Int3 &v) {
    if (SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = v.initSimd();
        __m128i ret = _mm_sub_epi32(data0, data1);
        x = _mm_extract_epi32(ret, 0);
        y = _mm_extract_epi32(ret, 1);
        z = _mm_extract_epi32(ret, 2);
        return;
    }
    x -= v.x; y -= v.y; z -= v.z;
}

void Int3::operator *= (const Int3 &v) {
    if (SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = v.initSimd();
        __m128i ret0 = _mm_mul_epi32(data0, data1);
        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        x = _mm_extract_epi32(ret0, 0);
        y = _mm_extract_epi32(ret1, 1);
        z = _mm_extract_epi32(ret0, 2);
        return;
    }
    x *= v.x; y *= v.y; z *= v.z;
}

void Int3::operator *= (const int factor) {
  if (SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = _mm_set1_epi32(factor);
        __m128i ret0 = _mm_mul_epi32(data0, data1);
        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        x = _mm_extract_epi32(ret0, 0);
        y = _mm_extract_epi32(ret1, 1);
        z = _mm_extract_epi32(ret0, 2);
        return;
    }
    x *= factor; y *= factor; z *= factor;   
}

void Int3::operator /= (const Int3 &v) {
    if(SIMD_ENABLED == 1){
        x /= v.x; y /= v.y; z /= v.z;
        return;
    }
    x /= v.x; y /= v.y; z /= v.z;
}

void Int3::operator /= (const int factor) {
    if(SIMD_ENABLED == 1){
        x /= factor; y /= factor; z /= factor;
        return;
    }
    x /= factor; y /= factor; z /= factor;   
}

bool Int3::operator == (const Int3 &v) {
    return x==v.x && y==v.y && z==v.z;
}

Int3 operator + (const Int3 &v0, const Int3 &v1) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret = _mm_add_epi32(data0, data1);
        
        return Int3(_mm_extract_epi32(ret, 0),
                    _mm_extract_epi32(ret, 1),
                    _mm_extract_epi32(ret, 2));
    }
    return Int3(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
}

Int3 operator - (const Int3 &v0, const Int3 &v1) {
   if(SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret = _mm_sub_epi32(data0, data1);
        
        return Int3(_mm_extract_epi32(ret, 0),
                    _mm_extract_epi32(ret, 1),
                    _mm_extract_epi32(ret, 2));
    }
    return Int3(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
}

Int3 operator * (const Int3 &v0, const Int3 &v1) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret0 = _mm_mul_epi32(data0, data1);
        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        return Int3(_mm_extract_epi32(ret0, 0),
                    _mm_extract_epi32(ret1, 1),
                    _mm_extract_epi32(ret0, 2));
    }
    return Int3(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z);
}

Int3 operator * (const Int3 &v, const int factor) {
    return v*Int3(factor, factor, factor);
}

Int3 operator * (const int factor, const Int3 &v) {
    return v*Int3(factor, factor, factor);
}

Int3 operator / (const Int3 &v0, const Int3 &v1) {
    if(SIMD_ENABLED == 1){}
    return Int3(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z);
}

Int3 operator / (const Int3 &v, const int factor) {
    return Int3(v.x/factor, v.y/factor, v.z/factor);
}

void Int3::copyTo(int *dstData){
    memcpy(dstData, data, sizeof(int)*3);
}

#pragma mark Int4

Int4::Int4(int a, int b, int c, int d) {
    x = a; y = b; z = c; w = d;
}

inline __m128i Int4::initSimd() {
    return _mm_setr_epi32(x, y, z, w);
}

inline __m128i Int4::initSimd() const {
    return _mm_setr_epi32(x, y, z, w);
}

bool Int4::operator == (const Int4 &v) {
    return x==v.x && y==v.y && z==v.z && w==v.w;
}

void Int4::operator = (const Int4 &v) {
    x=v.x; y=v.y; z=v.z; w=v.w;
}

void Int4::operator += (const Int4 &v) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = v.initSimd();
        __m128i ret = _mm_add_epi32(data0, data1);
        x = _mm_extract_epi32(ret, 0);
        y = _mm_extract_epi32(ret, 1);
        z = _mm_extract_epi32(ret, 2);
        w = _mm_extract_epi32(ret, 3);
        return;
    }
    x += v.x; y += v.y;
    z += v.z; w += v.w;
}
 
void Int4::operator -= (const Int4 &v) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = v.initSimd();
        __m128i ret = _mm_sub_epi32(data0, data1);
        x = _mm_extract_epi32(ret, 0);
        y = _mm_extract_epi32(ret, 1);
        z = _mm_extract_epi32(ret, 2);
        w = _mm_extract_epi32(ret, 3);
        return;
    }
    x -= v.x; y -= v.y;
    z -= v.z; w -= v.w;
}

void Int4::operator *= (const Int4 &v) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = v.initSimd();
        __m128i ret0 = _mm_mul_epi32(data0, data1);
        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        x = _mm_extract_epi32(ret0, 0);
        y = _mm_extract_epi32(ret1, 1);
        z = _mm_extract_epi32(ret0, 2);
        w = _mm_extract_epi32(ret1, 3);
        return;
    }
    x *= v.x; y *= v.y;
    z *= v.z; w *= v.w;
}

void Int4::operator *= (const int factor) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = this->initSimd();
        __m128i data1 = _mm_set1_epi32(factor);
        __m128i ret0 = _mm_mul_epi32(data0, data1);
        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        x = _mm_extract_epi32(ret0, 0);
        y = _mm_extract_epi32(ret1, 1);
        z = _mm_extract_epi32(ret0, 2);
        w = _mm_extract_epi32(ret1, 3);
        return;
    }
    x *= factor; y *= factor;
    z *= factor; w *= factor;
}

void Int4::operator /= (const Int4 &v){
    if(SIMD_ENABLED == 1){
        x /= v.x; y /= v.y;
        z /= v.z; w /= v.w;
        return;
    }
    x /= v.x; y /= v.y;
    z /= v.z; w /= v.w;
}

void Int4::operator /= (const int factor) {
   if(SIMD_ENABLED == 1){
        x /= factor; y /= factor;
        z /= factor; w /= factor;
        return;
    }
    x /= factor; y /= factor;
    z /= factor; w /= factor;
}

Int4 operator + (const Int4 &v0, const Int4 &v1) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret = _mm_add_epi32(data0, data1);
        return Int4(_mm_extract_epi32(ret, 0),
                    _mm_extract_epi32(ret, 1),
                    _mm_extract_epi32(ret, 2),
                    _mm_extract_epi32(ret, 3));
    }
    return Int4(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z, v0.w+v1.w); 
}

Int4 operator - (const Int4 &v0, const Int4 &v1) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret = _mm_sub_epi32(data0, data1);
        return Int4(_mm_extract_epi32(ret, 0),
                    _mm_extract_epi32(ret, 1),
                    _mm_extract_epi32(ret, 2),
                    _mm_extract_epi32(ret, 3));
    }
    return Int4(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z, v0.w-v1.w);
}

Int4 operator * (const Int4 &v0, const Int4 &v1) {
    if (SIMD_ENABLED == 1){
        __m128i data0 = v0.initSimd();
        __m128i data1 = v1.initSimd();
        __m128i ret0 = _mm_mul_epi32(data0, data1);
        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        return Int4(_mm_extract_epi32(ret0, 0),
                    _mm_extract_epi32(ret1, 1),
                    _mm_extract_epi32(ret0, 2),
                    _mm_extract_epi32(ret1, 3));
    }
    return Int4(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z, v0.w*v1.w);
}

Int4 operator * (const Int4 &v, const int factor) {
    if(SIMD_ENABLED == 1){
        __m128i data0 = v.initSimd();
        __m128i data1 = _mm_set1_epi32(factor);
        __m128i ret0 = _mm_mul_epi32(data0, data1);
        __m128i ret1 = _mm_mullo_epi32(data0, data1);
        return Int4(_mm_extract_epi32(ret0, 0),
                    _mm_extract_epi32(ret1, 1),
                    _mm_extract_epi32(ret0, 2),
                    _mm_extract_epi32(ret1, 3));
    }
    return Int4(v.x*factor, v.y*factor, v.z*factor, v.w*factor);
}

Int4 operator * (const int factor, const Int4 &v) {
    return v*factor;
}

Int4 operator / (const Int4 &v0, const Int4 &v1) {
    return Int4(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z, v0.w/v1.w);
}

Int4 operator / (const Int4 &v, const int factor) {
    return Int4(v.x/factor, v.y/factor, v.z/factor, v.w/factor);
}

std::string Int4::desc(){
    char buf[512];
    sprintf(buf, "%d, %d, %d, %d", data[0], data[1], data[2], data[3]);
    return std::string(buf);
}

void Int4::copyTo(int *dstData){
    memcpy(dstData, data, sizeof(int)*4);
}

#pragma mark Float2
Float2::Float2(float a, float b) {
    x = a; y = b;
}

Float2::Float2(const Float2 &v){
    x = v.x; y = v.y;
}

std::string Float2::desc() {
    char buf[256];
    sprintf(buf, "[%f, %f]", x, y);
    std::string str = std::string(buf);
    return str;
}

void Float2::copyTo(float *dstData){
    memcpy(dstData, data, sizeof(float)*2);
}

__m128 Float2::initSimd() {
    return _mm_setr_ps(x, y, 0.0, 0.0);
}

__m128 Float2::initSimd() const {
    return _mm_setr_ps(x, y, 0.0, 0.0);
}

float Float2::dot(const Float2 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), v.initSimd());
        return ret[0] + ret[1];
    }
    return x*v.x+y*v.y;
}

bool Float2::operator == (const Float2 &v) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_cmpeq_ps(this->initSimd(), v.initSimd());
        return ret[0] == ret[1] == 0.0;
    }
    return x==v.x && y==v.y;
}

void Float2::operator = (const Float2 &v) {
    x = v.x; y = v.y;
}

void Float2::operator += (const Float2 &v) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_add_ps(this->initSimd(), v.initSimd());
        x = ret[0];
        y = ret[1];
        return;
    }
    x += v.x; y += v.y;
}

void Float2::operator -= (const Float2 &v) {
   if(SIMD_ENABLED == 1){
        __m128 ret = _mm_sub_ps(this->initSimd(), v.initSimd());
        x = ret[0];
        y = ret[1];
        return;
    }
    x -= v.x; y -= v.y;
}

void Float2::operator *= (const Float2 &v) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1];
        return;
    }
    x *= v.x; y *= v.y;
}

void Float2::operator *= (const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), _mm_set1_ps(factor));
        x = ret[0]; y = ret[1];
        return;
    }
    x *= factor; y *= factor;
}

void Float2::operator /= (const Float2 &v) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1];
        return;
    }
    x /= v.x; y /= v.y;
}

void Float2::operator /= (const float factor) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(this->initSimd(), _mm_set1_ps(factor));
        x = ret[0]; y = ret[1];
        return;
    }
    x /= factor; y /= factor;
}

Float2 operator + (const Float2 &v0, const Float2 &v1) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_add_ps(v0.initSimd(), v1.initSimd());
        return Float2(ret[0], ret[1]);
    }
    return Float2(v0.x+v1.x, v0.y+v1.y);
}

Float2 operator - (const Float2 &v0, const Float2 &v1) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_sub_ps(v0.initSimd(), v1.initSimd());
        return Float2(ret[0], ret[1]);
    }
    return Float2(v0.x-v1.x, v0.y-v1.y);
}

Float2 operator * (const Float2 &v0, const Float2 &v1) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v0.initSimd(), v1.initSimd());
        return Float2(ret[0], ret[1]);
    }
    return Float2(v0.x*v1.x, v0.y*v1.y);
}

Float2 operator * (const float factor, const Float2 &v) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float2(ret[0], ret[1]);
    }
    return Float2(v.x*factor, v.y*factor);
}

Float2 operator * (const Float2 &v, const float factor) {
    return factor*v;
}

Float2 operator / (const Float2 &v0, const Float2 &v1) {
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(v0.initSimd(), v1.initSimd());
        return Float2(ret[0], ret[1]);
    }
    return Float2(v0.x/v1.x, v0.y/v1.y);
}

Float2 operator / (const Float2 &v, const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float2(ret[0], ret[1]);
    }
    return Float2(v.x/factor, v.y/factor);
}

void Float2::normalized(){
    float len = sqrt(x*x + y*y);
    x /= len; y/=len;
}

Float2 Float2::normal() const{
    float len = sqrt(x*x + y*y);
    return Float2(x/len, y/len);
}

#pragma mark Float3
Float3::Float3(float a, float b, float c){
    x = a; y = b; z = c;
}

Float3::Float3(const Float3 &v){
    x=v.x; y=v.y; z=v.z;
}

std::string Float3::desc(){
    char buf[256];
    sprintf(buf, "[%f, %f, %f]", x, y, z);
    return std::string(buf);
}

void Float3::copyTo(float *dstData){
    memcpy(dstData, data, sizeof(float)*3);
}

inline __m128 Float3::initSimd() {
    return _mm_setr_ps(x, y, z, 0.0);
}

inline __m128 Float3::initSimd() const {
    return _mm_setr_ps(x, y, z, 0.0);
}

float Float3::dot(const Float3 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), v.initSimd());
        return ret[0] + ret[1] + ret[2];
    }
    return x*v.x + y*v.y + z*v.z;
}

bool Float3::operator == (const Float3 &v){
   if(SIMD_ENABLED == 1){
        __m128 ret = _mm_cmpeq_ps(this->initSimd(), v.initSimd());
        return ret[0] == ret[1] == ret[2] == 0.0;
    }
    return x==v.x && y==v.y && z==v.z;
}

void Float3::operator =  (const Float3 &v){
    x=v.x; y=v.y; z=v.z;
}

void Float3::operator += (const Float3 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_add_ps(this->initSimd(), v.initSimd());
        x=ret[0]; y=ret[1]; z=ret[2];
        return;
    }
    x+=v.x; y+=v.y; z+=v.z;
}

void Float3::operator -= (const Float3 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_sub_ps(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x-=v.x; y-=v.y; z-=v.z;
}

void Float3::operator *= (const Float3 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x*=v.x; y*=v.y; z*=v.z;
}

void Float3::operator *= (const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), _mm_set1_ps(factor));
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x*=factor; y*=factor; z*=factor;
}

void Float3::operator /= (const Float3 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x/=v.x; y/=v.y; z/=v.z;
}

void Float3::operator /= (const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(this->initSimd(), _mm_set1_ps(factor));
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x/=factor; y/=factor; z/=factor;
}

Float3 operator + (const Float3 &v0, const Float3 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_add_ps(v0.initSimd(), v1.initSimd());
        return Float3(ret[0], ret[1], ret[2]);
    }
    return Float3(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
}

Float3 operator - (const Float3 &v0, const Float3 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_sub_ps(v0.initSimd(), v1.initSimd());
        return Float3(ret[0], ret[1], ret[2]);
    }
    return Float3(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
}

Float3 operator * (const Float3 &v0, const Float3 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v0.initSimd(), v1.initSimd());
        return Float3(ret[0], ret[1], ret[2]);
    }
    return Float3(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z);
}

Float3 operator * (const float factor, const Float3 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float3(ret[0], ret[1], ret[2]);
    }
    return Float3(v.x*factor, v.y*factor, v.z*factor);
}

Float3 operator * (const Float3 &v, const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float3(ret[0], ret[1], ret[2]);
    }
    return Float3(v.x*factor, v.y*factor, v.z*factor);
}

Float3 operator / (const Float3 &v0, const Float3 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(v0.initSimd(), v1.initSimd());
        return Float3(ret[0], ret[1], ret[2]);
    }
    return Float3(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z);
}

Float3 operator / (const Float3 &v, const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float3(ret[0], ret[1], ret[2]);
    }
    return Float3(v.x/factor, v.y/factor, v.z/factor);
}

void Float3::normalized(){
    Float3 ret = Float3(x, y, z) * Float3(x, y, z);
    float len = sqrt(ret.x + ret.y + ret.z);
    x /= len; y /= len; z /= len;
}

Float3 Float3::normal() const{
    Float3 ret = Float3(x, y, z) * Float3(x, y, z);
    float len = sqrt(ret.x + ret.y + ret.z);
    return Float3(x/len, y/len, z/len);
}

Float3 Float3::cross(const Float3 &value){
    return Float3(y*value.z-z*value.y,
                  z*value.x-x*value.z,
                  x*value.y-y*value.x);
}

#pragma mark Float4
Float4::Float4(float a, float b, float c, float d){
    x=a; y=b; z=c; w=d;
}

Float4::Float4(const Float4 &v){
    x=v.x; y=v.y; z=v.z; w=v.w;
}

std::string Float4::desc(){
    char buf[256];
    sprintf(buf, "[%f, %f, %f, %f]", x, y, z, w);
    return std::string(buf);
}

void Float4::copyTo(float *dstData){
    memcpy(dstData, data, sizeof(float)*4);
}

inline __m128 Float4::initSimd() {
    return _mm_setr_ps(x, y, z, w);
}

inline __m128 Float4::initSimd() const {
    return _mm_setr_ps(x, y, z, w);
}

float Float4::dot(const Float4 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), v.initSimd());
        return ret[0] + ret[1] + ret[2] + ret[3];
    }
    return x*v.x + y*v.y + z*v.z + w*v.w;
}

bool Float4::operator == (const Float4 &v){
   if(SIMD_ENABLED == 1){
        __m128 ret = _mm_cmpeq_ps(this->initSimd(), v.initSimd());
        return ret[0] == ret[1] == ret[2] == ret[3] == 0;
    }
    return x==v.x && y==v.y && z==v.z;
}

void Float4::operator =  (const Float4 &v){
    x=v.x; y=v.y; z=v.z; w=v.w;
}

void Float4::operator += (const Float4 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_add_ps(this->initSimd(), v.initSimd());
        x=ret[0]; y=ret[1]; z=ret[2]; w=ret[3];
    }
    x+=v.x; y+=v.y; z+=v.z; w+=v.w;
}

void Float4::operator -= (const Float4 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_sub_ps(this->initSimd(), v.initSimd());
        x=ret[0]; y=ret[1]; z=ret[2]; w=ret[3];
    }
    x-=v.x; y-=v.y; z-=v.z; w-=v.w;
}

void Float4::operator *= (const Float4 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), v.initSimd());
        x=ret[0]; y=ret[1]; z=ret[2]; w=ret[3];
    }
    x*=v.x; y*=v.y; z*=v.z; w*=v.w;
}

void Float4::operator *= (const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(this->initSimd(), _mm_set1_ps(factor));
        x=ret[0]; y=ret[1]; z=ret[2]; w=ret[3];
    }
    x*=factor; y*=factor; z*=factor; w*=factor;
}

void Float4::operator /= (const Float4 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(this->initSimd(), v.initSimd());
        x=ret[0]; y=ret[1]; z=ret[2]; w=ret[3];
    }
    x/=v.x; y/=v.y; z/=v.z; w/=v.w;
}

void Float4::operator /= (const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(this->initSimd(), _mm_set1_ps(factor));
        x=ret[0]; y=ret[1]; z=ret[2]; w=ret[3];
    }
    x/=factor; y/=factor; z/=factor; w/=factor;
}

Float4 operator + (const Float4 &v0, const Float4 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_add_ps(v0.initSimd(), v1.initSimd());
        return Float4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Float4(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z, v0.w+v1.w);
}

Float4 operator - (const Float4 &v0, const Float4 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_sub_ps(v0.initSimd(), v1.initSimd());
        return Float4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Float4(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z, v0.w-v1.w);
}

Float4 operator * (const Float4 &v0, const Float4 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v0.initSimd(), v1.initSimd());
        return Float4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Float4(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z, v0.w*v1.w);
}

Float4 operator * (const float factor, const Float4 &v){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Float4(v.x*factor, v.y*factor, v.z*factor, v.w*factor);
}

Float4 operator * (const Float4 &v, const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_mul_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Float4(v.x*factor, v.y*factor, v.z*factor, v.w*factor);
}

Float4 operator / (const Float4 &v0, const Float4 &v1){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(v0.initSimd(), v1.initSimd());
        return Float4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Float4(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z, v0.w/v1.w);
}

Float4 operator / (const Float4 &v, const float factor){
    if(SIMD_ENABLED == 1){
        __m128 ret = _mm_div_ps(v.initSimd(), _mm_set1_ps(factor));
        return Float4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Float4(v.x/factor, v.y/factor, v.z/factor, v.w/factor);
}

void Float4::normalized(){
    Float4 ret = Float4(x,y,z,w) * Float4(x,y,z,w);
    float len = sqrt(ret.x + ret.y + ret.z + ret.w);
    x /= len; y /= len; z /= len; w /= len;
}

Float4 Float4::normal() const{
    Float4 ret = Float4(x,y,z,w) * Float4(x,y,z,w);
    float len = sqrt(ret.x + ret.y + ret.z + ret.w);
    return Float4(x/len, y/len, z/len, w/len);
}

#pragma mark Double2
Double2::Double2(double a, double b) {
    x = a; y = b;
}

Double2::Double2(const Double2 &v) {
    x = v.x; y = v.y;
}

std::string Double2::desc(){
    char buf[256];
    sprintf(buf, "[%lf, %lf]", x, y);
    return std::string(buf);
}

void Double2::copyTo(double *dstData){
    memcpy(dstData, data, sizeof(double)*2);
}

inline __m128d Double2::initSimd() {
    return _mm_setr_pd(x, y);
}

inline __m128d Double2::initSimd() const {
    return _mm_setr_pd(x, y);
}

double Double2::dot(const Double2 &v){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_mul_pd(this->initSimd(), v.initSimd());
        return ret[0] + ret[1];
    }
    return x*v.x + y*v.y;
}

bool Double2::operator == (const Double2 &v) {
    return x==v.x && y==v.y;
}

void Double2::operator = (const Double2 &v) {
    x=v.x; y=v.y;
}

void Double2::operator += (const Double2 &v){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_add_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y=ret[1];
        return;
    }
    x+=v.x; y+=v.y;
}

void Double2::operator -= (const Double2 &v){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_sub_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y=ret[1];
        return;
    }
    x-=v.x; y-=v.y;
}

void Double2::operator *= (const Double2 &v){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_mul_pd(this->initSimd(), v.initSimd());
        x=ret[0]; y=ret[1];
        return;
    }
    x *= v.x; y *= v.y;
}

void Double2::operator *= (const double factor){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_mul_pd(this->initSimd(), _mm_set1_pd(factor));
        x = ret[0]; y = ret[1];
        return;
    }
    x *= factor; y *= factor;
}

void Double2::operator /= (const Double2 &v){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_div_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1];
    }
    x /= v.x; y /= v.y;
}

void Double2::operator /= (const double factor){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_div_pd(this->initSimd(), _mm_set1_pd(factor));
        x = ret[0]; y = ret[1];
    }
    x /= factor; y /= factor;
}

Double2 operator + (const Double2 &v0, const Double2 &v1){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_add_pd(v0.initSimd(), v1.initSimd());
        return Double2(ret[0], ret[1]);
    }
    return Double2(v0.x+v1.x, v0.y+v1.y);
}

Double2 operator - (const Double2 &v0, const Double2 &v1){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_sub_pd(v0.initSimd(), v1.initSimd());
        return Double2(ret[0], ret[1]);
    }
    return Double2(v0.x-v1.x, v0.y-v1.y);
}

Double2 operator * (const Double2 &v0, const Double2 &v1){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_mul_pd(v0.initSimd(), v1.initSimd());
        return Double2(ret[0], ret[1]);
    }
    return Double2(v0.x*v1.x, v0.y*v1.y);
}

Double2 operator * (const Double2 &v, const double factor){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_mul_pd(v.initSimd(), _mm_set1_pd(factor));
        return Double2(ret[0], ret[1]);
    }
    return Double2(v.x*factor, v.y*factor);
}

Double2 operator * (const double factor, const Double2 &v){
    return v*factor;
}

Double2 operator / (const Double2 &v0, const Double2 &v1){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_div_pd(v0.initSimd(), v1.initSimd());
        return Double2(ret[0], ret[1]);
    }
    return Double2(v0.x/v1.x, v0.y/v1.y);
}

Double2 operator / (const Double2 &v, const double factor){
    if(SIMD_ENABLED == 1){
        __m128d ret = _mm_div_pd(v.initSimd(), _mm_set1_pd(factor));
        return Double2(ret[0], ret[1]);
    }
    return Double2(v.x/factor, v.y/factor);
}  

void Double2::normalized() {
    Double2 ret = Double2(x, y)*Double2(x, y);
    double len = sqrt(ret.x + ret.y);
    x /= len; y/= len;
}

Double2 Double2::normal() const {
    Double2 ret = Double2(x, y)*Double2(x, y);
    double len = sqrt(ret.x + ret.y);
    return Double2(x/len, y/len);
}

#pragma mark Double3
Double3::Double3(double a, double b, double c){
    x=a; y=b; z=c;
}

Double3::Double3(const Double3 &v){
    x=v.x; y=v.y; z=v.z;
}

std::string Double3::desc(){
    char buf[256];
    sprintf(buf, "[%lf, %lf, %lf]", x, y, z);
    return std::string(buf);
}

void Double3::copyTo(double *dstData){
    memcpy(dstData, data, sizeof(double)*3);
}

void Double3::normalized(){
    Double3 ret = Double3(x,y,z)*Double3(x,y,z);
    double len = sqrt(ret.x + ret.y + ret.z);
    x/=len; y/=len; z/=len;
}

Double3 Double3::normal() const {
    Double3 ret = Double3(x,y,z)*Double3(x,y,z);
    double len = sqrt(ret.x + ret.y + ret.z);
    return Double3(x/len, y/len, z/len);
}

Double3 Double3::cross(const Double3 &value){
    return Double3(y*value.z-z*value.y,
                   z*value.x-x*value.z,
                   x*value.y-y*value.x);
}

inline __m256d Double3::initSimd(){
    return _mm256_setr_pd(x, y, z, 0.0);
}

inline __m256d Double3::initSimd() const {
    return _mm256_setr_pd(x, y, z, 0.0);
}

double Double3::dot(const Double3 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(this->initSimd(), v.initSimd());
        return ret[0] + ret[1] + ret[2];
    }
    return x*v.x + y*v.y + z*v.z;
}

bool Double3::operator == (const Double3 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_cmp_pd(this->initSimd(), v.initSimd(), _CMP_NEQ_OS);
        return ret[0] == ret[1] == ret[2] == 0; 
    }
    return x==v.x && y==v.y && z==v.z;
}

void Double3::operator =  (const Double3 &v){
    x=v.x; y=v.y; z=v.z;
}

void Double3::operator += (const Double3 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_add_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x+=v.x; y+=v.y; z+=v.z;
}

void Double3::operator -= (const Double3 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_sub_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x-=v.x; y-=v.y; z-=v.z;
}

void Double3::operator *= (const Double3 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x*=v.x; y*=v.y; z*=v.z;
}

void Double3::operator *= (const double factor){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(this->initSimd(), _mm256_set1_pd(factor));
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x*=factor; y*=factor; z*=factor;
}

void Double3::operator /= (const Double3 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x/=v.x; y/=v.y; z/=v.z;
}

void Double3::operator /= (const double factor){
   if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(this->initSimd(), _mm256_set1_pd(factor));
        x = ret[0]; y = ret[1]; z = ret[2];
        return;
    }
    x/=factor; y/=factor; z/=factor;
}

Double3 operator + (const Double3 &v0, const Double3 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_add_pd(v0.initSimd(), v1.initSimd());
        return Double3(ret[0], ret[1], ret[2]);
    }
    return Double3(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
}

Double3 operator - (const Double3 &v0, const Double3 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_sub_pd(v0.initSimd(), v1.initSimd());
        return Double3(ret[0], ret[1], ret[2]);
    }
    return Double3(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
}

Double3 operator * (const Double3 &v0, const Double3 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(v0.initSimd(), v1.initSimd());
        return Double3(ret[0], ret[1], ret[2]);
    }
    return Double3(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z);
}

Double3 operator * (const Double3 &v, const double factor){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(v.initSimd(), _mm256_set1_pd(factor));
        return Double3(ret[0], ret[1], ret[2]);
    }
    return Double3(v.x*factor, v.y*factor, v.z*factor);
}

Double3 operator * (const double factor, const Double3 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(v.initSimd(), _mm256_set1_pd(factor));
        return Double3(ret[0], ret[1], ret[2]);
    }
    return Double3(v.x*factor, v.y*factor, v.z*factor);
}

Double3 operator / (const Double3 &v0, const Double3 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(v0.initSimd(), v1.initSimd());
        return Double3(ret[0], ret[1], ret[2]);
    }
    return Double3(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z);
}

Double3 operator / (const Double3 &v, const double factor){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(v.initSimd(), _mm256_set1_pd(factor));
        return Double3(ret[0], ret[1], ret[2]);
    }
    return Double3(v.x/factor, v.y/factor, v.z/factor);
}

#pragma mark Double4
Double4::Double4(double a, double b ,double c, double d){
    x = a; y = b; z = c; w = d;
}

Double4::Double4(const Double4 &v){
    x=v.x; y=v.y; z=v.z; w=v.w;
}

void Double4::normalized(){
    Double4 ret = Double4(x,y,z,w)*Double4(x,y,z,w);
    double len = sqrt(ret.x+ret.y+ret.z+ret.w);
    x /= len; y /= len; z /= len; w /= len;
}

Double4 Double4::normal() const {
    Double4 ret = Double4(x,y,z,w)*Double4(x,y,z,w);
    double len = sqrt(ret.x+ret.y+ret.z+ret.w);
    return Double4(x/len, y/len, z/len, w/len);
}

double Double4::dot(const Double4 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(v.initSimd(), this->initSimd());
        return ret[0] + ret[1] + ret[2] + ret[3];
    }
    return v.x*x + v.y*y + v.z*z + v.w*w;
}

std::string Double4::desc(){
    char buf[256];
    sprintf(buf, "[%lf, %lf, %lf, %lf]", x, y ,z, w);
    return std::string(buf);
}

void Double4::copyTo(double *dstData){
    memcpy(dstData, data, sizeof(double)*4);
}

inline __m256d Double4::initSimd(){
    return _mm256_setr_pd(x, y, z, w);
}

inline __m256d Double4::initSimd() const {
    return _mm256_setr_pd(x, y, z, w);
}

bool Double4::operator == (const Double4 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_cmp_pd(this->initSimd(), v.initSimd(), _CMP_NEQ_OS);
        return ret[0] == ret[1] == ret[2] == ret[3] == 0; 
    }
    return x==v.x && y==v.y && z==v.z && w==v.w;
}

void Double4::operator =  (const Double4 &v){
    x=v.x; y=v.y; z=v.z; w=v.w;
}

void Double4::operator += (const Double4 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_add_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2]; w = ret[3];
    }
    x+=v.x; y+=v.y; z+=v.z; w+=v.w;
}

void Double4::operator -= (const Double4 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_sub_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2]; w = ret[3];
    }
    x-=v.x; y-=v.y; z-=v.z; w-=v.w;
}

void Double4::operator *= (const Double4 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2]; w = ret[3];
    }
    x*=v.x; y*=v.y; z*=v.z; w*=v.w;
}

void Double4::operator *= (const double factor){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(this->initSimd(), _mm256_set1_pd(factor));
        x = ret[0]; y = ret[1]; z = ret[2]; w = ret[3];
    }
    x*=factor; y*=factor; z*=factor; w*=factor;
}

void Double4::operator /= (const Double4 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(this->initSimd(), v.initSimd());
        x = ret[0]; y = ret[1]; z = ret[2]; w = ret[3];
    }
    x/=v.x; y/=v.y; z/=v.z; w/=v.w;
}

void Double4::operator /= (const double factor){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(this->initSimd(), _mm256_set1_pd(factor));
        x = ret[0]; y = ret[1]; z = ret[2]; w = ret[3];
    }
    x/=factor; y/=factor; z/=factor; w/=factor;
}


Double4 operator + (const Double4 &v0, const Double4 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_add_pd(v0.initSimd(), v1.initSimd());
        return Double4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Double4(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z, v0.w+v1.w);
}

Double4 operator - (const Double4 &v0, const Double4 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_sub_pd(v0.initSimd(), v1.initSimd());
        return Double4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Double4(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z, v0.w-v1.w);
}

Double4 operator * (const Double4 &v0, const Double4 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(v0.initSimd(), v1.initSimd());
        return Double4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Double4(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z, v0.w*v1.w);
}

Double4 operator * (const Double4 &v, const double factor){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(v.initSimd(), _mm256_set1_pd(factor));
        return Double4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Double4(v.x*factor, v.y*factor, v.z*factor, v.w*factor);
}

Double4 operator * (const double factor, const Double4 &v){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_mul_pd(v.initSimd(), _mm256_set1_pd(factor));
        return Double4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Double4(v.x*factor, v.y*factor, v.z*factor, v.w*factor);
}

Double4 operator / (const Double4 &v0, const Double4 &v1){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(v0.initSimd(), v1.initSimd());
        return Double4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Double4(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z, v0.w/v1.w);
}

Double4 operator / (const Double4 &v, const double factor){
    if(SIMD_ENABLED == 1){
        __m256d ret = _mm256_div_pd(v.initSimd(), _mm256_set1_pd(factor));
        return Double4(ret[0], ret[1], ret[2], ret[3]);
    }
    return Double4(v.x/factor, v.y/factor, v.z/factor, v.w/factor);
}




