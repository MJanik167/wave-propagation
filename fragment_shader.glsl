#version 330

out vec4 FragColor;
in vec3 outcol;
in vec2 iResolution;

float sdBox( in vec2 p, in vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float map(vec3 point){
	float okop1 = sdBox(vec2(.5,0.), vec2(0.));
	float okop2 = sdBox(point.xy, vec2(0.5,0.5));

	return okop1;
}

void main()
{
	vec2 uv = (gl_FragCoord.xy*2.-iResolution.xy)/iResolution.y;

	vec3 ro = vec3(0.,0.,-3.);
	vec3 rd = normalize(vec3(uv.xy,1.));
	
	int i;
	float t=0.;
	float d=0.;

	for(i = 0; i<40; i++){
		vec3 p = ro + rd*t;


		float d = map(p);

		t += d;
		if(d<0.01 || t>100.){
			break;
		}
	}

	vec3 color = vec3(t*0.1);

	gl_FragColor  = vec4(gl_FragCoord.zzz, 1.0f);
	//gl_FragColor  = vec4(1.0f, 0.0f,0.0f, 1.0f);

} 
