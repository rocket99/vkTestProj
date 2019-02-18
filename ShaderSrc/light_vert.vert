//
//light_vert.vert
//2019-1-2

#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set=0, binding=0) uniform viewPara {
    vec3 space_size;
    mat4 projView_mat;
}camera;

layout(set=0, binding=1) uniform objMat {
    mat4 trans_mat;
    mat4 rotate_mat;
}objPara;

layout(location=0) in vec3 in_coord;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_color;
layout(location=3) in vec2 in_texCoord;

layout(location=0) out outVertice
{
    vec3 vert_coord;
    vec3 vert_normal;
    vec4 vert_color;
    vec2 vert_texCoord;
};

void main(){
    vec4 pos = objPara.trans_mat*vec4(in_coord / camera.space_size, 1.0);
    gl_Position = camera.projView_mat*pos;

    vert_coord = pos.xyz;
    vert_color = in_color;
    vert_normal = normalize(in_normal);
    vert_texCoord = in_texCoord;
}

