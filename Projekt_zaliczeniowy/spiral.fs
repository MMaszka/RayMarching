#version 120

uniform vec2 resolution;
uniform vec3 color;
uniform float iTime;
out vec4 FragColor;

in vec2 pos;

vec3 palette(float t){
    vec3 a = vec3 (0.2,0.4,0.2);
    vec3 b = vec3 (0.2,0.7,0.7);
    vec3 c = vec3 (0.2,0.8,0.9);
    vec3 d = vec3 (0.163,sin(iTime/2)/2+1,0.7257);
    return a + b*cos( 6.28318*(c*t+d));
}

mat2 rot2D(float angle){
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c,-s,s,c);
}

vec3 rot3D(vec3 p , vec3 axis, float angle){
    return mix(dot(axis, p)*axis,p,cos(angle)) + cross(axis,p) * sin(angle);

}


float smin( float a, float b, float k )
{
    float h = max( k-abs(a-b), 0.0 )/k;
    return min( a, b ) - h*h*k*(1.0/4.0);
}

// -- SHAPES

//box
float sdBox(vec3 position, vec3 size){
    vec3 q = abs(position)-size;

    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}
//sphere
float sdSphere(vec3 position, float size){
    return length(position)-size;
}
float sdOctahedron( vec3 position, float size){
    position = abs(position);
    return (position.x + position.y + position.z - size) * 0.57735027;
}

// SPACE
float map(vec3 position){

    vec3 Qposition = position;

    Qposition.z += iTime;

    Qposition.xy = mod(Qposition.xy,1.0) - 0.5;
    Qposition.z = mod(Qposition.z,0.25) - 0.125;

  
    
    float box = sdOctahedron(Qposition, 0.15);


    return box;
}

vec3 col = vec3(0);

// MAIN
void main(){

    vec2 uv = pos;
    uv.x *= resolution.x/resolution.y;
    float w =-iTime/2;

    vec3 RayOrigin = vec3(0.0,0.0,-3.0);
    vec3 RayDirection = normalize(vec3(uv.x*cos(w)+uv.y*sin(w),uv.y*cos(w)-uv.x*sin(w),1.0));

    float traveled =0.0;

    // ray behavior
    int i=0;
    for (i = 0; i < 80; i++){
        vec3 position = RayOrigin + RayDirection * traveled;

        position.xy *= rot2D(traveled*(sin(iTime/10)/5));

        position.y += sin(traveled+iTime*2)*0.35;
        position.x += cos(traveled+iTime*2)*0.35;

        float distance = map(position);

        traveled += distance;
        if(distance<0.001 || traveled > 100.) break;
    }

    col = palette(traveled*0.04+float(i)*0.005);

    FragColor = vec4(col,1.0);
}