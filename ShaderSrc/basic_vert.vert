//
//vertex shader
#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding=0) uniform viewMat {
    vec3 space_size;
    mat4 proj_mat;
}viewPara;

layout(binding=1) uniform objMat {
    mat4 trans_mat;
    mat4 rotate_mat;
}objPara;

layout(location=0) in vec3 in_coord;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_color;
layout(location=3) in vec2 in_texCoord;

layout(location=0) out outVertice
{
    vec4 vert_color;
    vec3 vert_normal;
};

void main() {
    vec4 pos = objPara.trans_mat*vec4(in_coord/viewPara.space_size, 1.0);
    gl_Position = viewPara.proj_mat * pos;
    
    vert_normal = normalize(objPara.rotate_mat * vec4(in_normal, 1.0)).xyz;
    vert_color  = vec4(1.0, 0.5, 0.6, 1.0);

}
