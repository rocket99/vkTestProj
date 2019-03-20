#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in outVertice {
    vec4 vert_color;
    vec3 vert_normal;
};

layout(location = 0) out vec4 fragColor;
void main(void){
    fragColor = vert_color;
}
