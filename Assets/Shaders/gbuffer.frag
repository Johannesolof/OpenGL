#version 450

//#include common.glsl

in vsData {
    vec3 wsPosition;
    vec3 wsNormal;
    vec3 wsTangent;
    vec3 wsBitanget;
    vec2 texCoord;
} vsData;

out vec4 outColor;

void main()
{
    
    outColor = vec4(vsData.wsNormal, 1.f); 
}