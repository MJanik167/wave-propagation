#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 incol;
layout (location = 2) in vec2 resolution;


uniform mat4 MVP;

out vec3 outcol;
out vec2 iResolution;

void main()
{
    iResolution = resolution;
    outcol=incol;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}