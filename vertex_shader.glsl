#version 330 

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aColor;


out float outcol;
out vec2 iResolution;

void main()
{
    outcol=aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0., 1.0);
}