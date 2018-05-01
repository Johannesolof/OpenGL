#version 430

uniform sampler2D inTexture;

in vec2 texCoord;

out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(inTexture, texCoord);
}