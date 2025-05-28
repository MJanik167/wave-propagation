#version 330

out vec4 FragColor;
in float outcol;

vec3 center1=vec3(160./600.-1.,285./600.,0.);
vec3 center2=vec3(440./600.,300./600.-1.,0.);

uniform float iTime;
uniform vec2 iResolution;


vec3 palette(float t){
    vec3 a = vec3(0.7,.32,.16);
    vec3 b = vec3(.1,.1,.2);
    vec3 c = vec3(.0,64.0,128.0);
    vec3 d = vec3(0.1,.55,.6);

    return a+b*cos(6.28318*(c*t+d));
}

float sdBox( in vec3 p, in vec3 b )
{
    vec3 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}


float maxZ = 37.6;
float minZ = -18.1;


void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution* 2.0 - 1.0;
     vec3 ro = vec3(0.,0.,-3.);
    vec3 rd = normalize(vec3(uv.x,uv.y,1.0));
    
    float t=0.;
    float d=0.;
    float e=0.;
    float et=0.;

    for(int i =0; i<40;i++){
        vec3 p = ro + rd*t;
        vec3 p2 = ro + rd*(et);

        d = min(sdBox(p-center1,vec3(1.25,.6,.1)),sdBox(p-center2,vec3(1.25,.6,.1)));
        e = min(sdBox(p2-center1,vec3(1.3,.7,.1)),sdBox(p2-center2,vec3(1.3,.7,.1)));
                
        
        t+=d;
        et+=e;

       

        if(d<0.001||t>100.) break;
    }
    
    //if(t>100.&&t<195.)t=100.0;
    if(step(et*0.09,1)<1)t=100.0;
    

    vec3 trench= vec3((t*0.1));
    vec3 normCol = vec3(outcol-minZ)/(maxZ-minZ); 


    vec3 color = mix(normCol.x==0?vec3(0.7,.32,.16):palette(normCol.x),trench,0.01);

    //vec3 color = vec3((outcol+17.)*0.01);
    //+(t*0.01)
    //color = mix(color,(outcol),sin(iTime/100));
    

    gl_FragColor = vec4(vec3(color), 1.0);
    //gl_FragColor = vec4(vec3(outcol+17.)*0.01 , 1.0);
}
