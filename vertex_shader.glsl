#version 330 

layout (location = 0) in vec3 aPos;

out float outcol;
out vec2 iResolution;

void main()
{
    outcol=aPos.z;
    gl_Position = vec4(aPos.x, aPos.y, 0., 1.0);
}