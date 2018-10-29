#version 450 core

layout (location = 0) in vec3 position;

layout (std140) uniform Camera 
{
    mat4 proj;
    mat4 invProj;
    mat4 view;
    mat4 invView;
    mat4 viewProj;
    mat4 invViewProj;
    vec4 wsPosition;
};

uniform mat4 modelMatrix;

void main()
{
    gl_Position = viewProj * modelMatrix * vec4(position, 1.0f);
}