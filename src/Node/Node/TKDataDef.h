//
//TKDataDef.h
//2018-11-30


#ifndef __TKDataDef_h__
#define __TKDataDef_h__

#include "../TKPrefix.h"
#include "../Math/TKMath.h"

struct TKVertice {
    float coord_x, coord_y, coord_z;
    float normal_x, normal_y, normal_z;
    float color_r, color_g, color_b, color_a;
    float texCoord_x, texCoord_y;
    
    TKVertice(Float3 coord, Float3 normal_vec, Float4 vert_color, Float2 tex_coord){
        coord_x = coord.x;
        coord_y = coord.y;
        coord_z = coord.z;

        normal_x = normal_vec.x;
        normal_y = normal_vec.y;
        normal_z = normal_vec.z;

        color_r = vert_color.x;
        color_g = vert_color.y;
        color_b = vert_color.z;
        color_a = vert_color.w;

        texCoord_x = tex_coord.x;
        texCoord_y = tex_coord.y;
    }
};

struct TKMaterial {
    float emission[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shiness;
    void mapToBuffer(float *buf){
        memcpy(&buf[0], emission, sizeof(float)*4);
        memcpy(&buf[4], ambient,  sizeof(float)*4);
        memcpy(&buf[8], diffuse,  sizeof(float)*4);
        memcpy(&buf[12],specular, sizeof(float)*4);
        memcpy(&buf[16], &shiness, sizeof(float));
    }
};

struct TKLightSource {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[3];
    float halfvector[3];
    float spotDirection[3];
    float spotExponent;
    float spotCosCutOff;
    void mapToBuffer(float *buf){
        memcpy(&buf[0], ambient,  sizeof(float)*4);
        memcpy(&buf[4], diffuse,  sizeof(float)*4);
        memcpy(&buf[8], specular, sizeof(float)*4);
        memcpy(&buf[12], position, sizeof(float)*3);
        memcpy(&buf[16], halfvector, sizeof(float)*3);
        memcpy(&buf[20], spotDirection, sizeof(float)*3);
        memcpy(&buf[24], &spotExponent, sizeof(float));
        memcpy(&buf[25], &spotCosCutOff, sizeof(float));
    }
};
#endif

