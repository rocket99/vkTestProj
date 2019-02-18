//
//light_frag.frag
//2019-1-2

#version 450

#extension GL_ARB_separate_shader_objects:enable
#extension GL_ARB_shading_language_420pack:enable

layout(set=0, binding=2) uniform materialFront {
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
}material[2];

layout(set=0, binding=3) uniform lightSource{
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    vec3  position;
    vec3  halfvector;
    vec3  spotDirection;
    float spotExponent;
    float spotCutOff;
}lightSrc;

layout(set=0, binding=4) uniform eyePara {
    vec3 position;
}eye;

layout(set=0, binding=5) uniform transPara {
    mat4 rotation;
}transform;

layout(location=0) in outVertice {
    vec3 vert_coord;
    vec3 vert_normal;
    vec4 vert_color;
    vec2 vert_texCoord;
};

layout(location=0) out vec4 out_color;

vec4 adsLightFunc(vec3 normal, vec3 coord, uint front){
    vec4 n0 = transform.rotation*vec4(normalize(normal), 1.0);
    vec3 norm = normalize(n0.xyz);
    vec3 lightv = normalize(lightSrc.position-coord);
    vec3 eyev   = normalize(eye.position - coord);

    vec3 ambient = lightSrc.ambient * material[front].ambient;

    float sDotN = max(0.0, dot(lightv, norm));
    vec3 diffuse = sDotN*lightSrc.diffuse * material[front].diffuse;

    vec3 specular = vec3(0.0);
    if(sDotN > 0.0){
        vec3 rLight = normalize(reflect(lightv, norm));
        specular = lightSrc.specular*material[front].specular;
        specular *= pow(max(0.0, dot(rLight, eyev)), material[front].shiness);
    }
    
    vec3 result = clamp(ambient+diffuse+specular, vec3(0.0), vec3(1.0));
    return vec4(result, 1.0);
}

void main(){
    uint front = 0;
    if(gl_FrontFacing == false){
        front = 1;
    }
    out_color = adsLightFunc(vert_normal, vert_coord, 0);
}




