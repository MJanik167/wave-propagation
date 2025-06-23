#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;              

uniform float iTime;


layout(std430, binding=0) buffer Pos{
    float[] p;
};

layout(std430, binding=1) buffer Pos1{
    float[] p_future;
};

layout(std430, binding=2) buffer Pos2{
    float[] p_past;
};

layout(std430, binding=3) buffer Pos3{
    float[] tablica;
};

const float pi = 3.14159265358979323846f;

const float fpeak = 30.f;

const float dtr = 0.00001f;

const float ds = 0.02f;

void main() {
    uint x_id = gl_GlobalInvocationID.x;
	uint y_id = gl_GlobalInvocationID.y;
	uint nx = gl_NumWorkGroups.x;
	uint nz = gl_NumWorkGroups.y;
	uint id  = x_id*nx+y_id;
	uint xcenter = 450;
	uint zcenter = nz-350;
	
	p_past[id] = 2.0 * p[id] - p_future[id] +
		((dtr * dtr) / (ds * ds)) * tablica[id] * tablica[id] *
		(p[id+1] + p[id-1] + p[id+nx] + p[id-nx] - 4.0 * p[id]);

	if(x_id == xcenter && y_id == zcenter){	
		float tf = iTime * dtr;
		float exp_result = exp(-(((pi * fpeak * (tf - (1.0f / fpeak))) * (pi * fpeak * (tf - (1.0f / fpeak))))));
		p_past[id] = p_past[id] + exp_result * (1.0 - 2.0 * ((pi * fpeak * (tf - (1.0 / fpeak))) * (pi * fpeak * (tf - (1.0 / fpeak)))));
	}


	if(y_id==0){
		p_past[id] = p[id+0] + p[id+ 1] - p_future[id+ 1] + tablica[id+ 0] * (dtr / ds) * (p[id+ 1] - p[id+ 0] - (p_future[id+ 2] - p_future[id+ 1]));
	}else if(y_id==nx-1){
		p_past[id] = p[id] + p[id-1] - p_future[id-1] + tablica[id] * (dtr / ds) * (p[id-1] - p[id] - (p_future[id-2] - p_future[id-1]));
	}
	else if(x_id==0){
		p_past[id] = p[id] + p[id+nx] - p_future[id+nx] + tablica[id] * (dtr / ds) * (p[id+nx] - p[id] - (p_future[id+2*nx] - p_future[id+nx]));
	}else if(x_id == nz-1){
		p_past[id] = p[id] + p[id-nz] - p_future[id-nz] + tablica[id] * (dtr / ds) * (p[id-nz] - p[id] - (p_future[id-2*nz] - p_future[id-nz]));
	}

}