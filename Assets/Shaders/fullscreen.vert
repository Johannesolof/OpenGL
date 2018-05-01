#version 430

#include "common.glsl"

out vec2 texCood;

void main()
{
    gl_Position = fullscreenTriangle(gl_VertexID, texCood);
}