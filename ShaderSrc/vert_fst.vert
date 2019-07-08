#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

void main(){
    vec2 pos[6] = vec2[6](vec2(0.0, 0.0), vec2(0.5, 0.5), vec2(0.5, 0.0),
                          vec2(0.0, 0.0), vec2(0.0,-0.5), vec2(0.5, 0.5));
    gl_Position = vec4( pos[gl_VertexIndex], 0.0, 1.0 );
}
