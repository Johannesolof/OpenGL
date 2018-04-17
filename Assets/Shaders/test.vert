#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoord;

layout (std140) uniform Camera 
{
    mat4 proj;
    mat4 view;
};

uniform mat4 modelMatrix;



out vec3 outNormal;

void main()
{
    outNormal = normal;
    gl_Position = proj * view * modelMatrix * vec4(position, 1.0f);
}