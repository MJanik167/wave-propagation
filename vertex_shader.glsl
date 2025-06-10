#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aColor;


out float outcol;
out vec2 iResolution;

void main()
{
    outcol=aPos.z;
    //if(aColor!=0.0f) outcol = aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0., 1.0);
}