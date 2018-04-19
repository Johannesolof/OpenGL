#version 450

//#include common.glsl

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoord;

layout (std140) uniform Camera 
{
    mat4 proj;
    mat4 invProj;
    mat4 view;
    mat4 invView;
    mat4 viewProj;
    mat4 invViewProj;
    vec4 wsPosition;
} camera;

uniform mat4 modelMatrix;

out vsData {
    vec3 wsPosition;
    vec3 wsNormal;
    vec3 wsTangent;
    vec3 wsBitanget;
    vec2 texCoord;
} vsData;

void main()
{
    vsData.wsPosition = (modelMatrix * vec4(position, 1.f)).xyz;
    vsData.wsNormal = normal;
    vsData.wsTangent = tangent;
    vsData.wsBitanget = bitangent;
    vsData.texCoord = texCoord;
    gl_Position = camera.viewProj * vec4(vsData.wsPosition, 1.f);
}