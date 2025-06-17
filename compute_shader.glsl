#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (location = 0) uniform float t;                 /** Time */

layout(std430, binding=0) buffer Pos{
    float[] pos;
};

void main() {
    uint id = gl_GlobalInvocationID.x;
    float p = pos[id];

    p+=0.001;

    pos[id] = p;
}