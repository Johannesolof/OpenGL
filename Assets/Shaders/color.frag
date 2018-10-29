#version 450

//#include common.glsl

uniform vec4 color;

out vec4 outColor;

void main()
{
    outColor = color; 
}