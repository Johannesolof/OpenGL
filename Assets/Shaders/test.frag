#version 450

#include common.glsl

in vec3 outNormal;

out vec4 outColor;

void main()
{
    
    outColor = vec4(outNormal, 1.f); 
}