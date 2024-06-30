#version 120

uniform vec2 resolution;
uniform float iTime;
uniform vec3 CamPos;

uniform vec4 quaternion;

out vec4 FragColor;

in vec2 pos;

mat3 quaternionRotationMatrix(vec4 Quaternion) {
	mat3 matrix = {
		{1- 2 * (Quaternion.z * Quaternion.z + Quaternion.w * Quaternion.w),        2 * (Quaternion.y * Quaternion.z - Quaternion.w * Quaternion.x),	2 * (Quaternion.y * Quaternion.w + Quaternion.z * Quaternion.x)},
		{   2 * (Quaternion.y * Quaternion.z + Quaternion.w * Quaternion.x),    1 - 2 * (Quaternion.y * Quaternion.y + Quaternion.w * Quaternion.w),	2 * (Quaternion.z * Quaternion.w - Quaternion.y * Quaternion.x)},
		{   2 * (Quaternion.y * Quaternion.w - Quaternion.z * Quaternion.x),        2 * (Quaternion.z * Quaternion.w + Quaternion.y * Quaternion.x),1 - 2 * (Quaternion.y * Quaternion.y + Quaternion.z * Quaternion.z)}
	};
	return matrix;
}
vec3 QuatMulti(vec3 p, vec4 q){
    vec4 multi = q;

    return p;
}

vec3 palette(float t){
    vec3 a = vec3 (0.2,0.4,0.2);
    vec3 b = vec3 (0.7,0.7,0.7);
    vec3 c = vec3 (0.7,0.8,0.9);
    vec3 d = vec3 (0.7,0.3,0.7257);
    return a + b*cos( 6.28318*(c*t+d));
}
vec3 palette2(float t){
    vec3 a = vec3 (0.0,0.0,0.0);
    vec3 b = vec3 (0.9,0.9,0.9);
    vec3 c = vec3 (0.1,0.1,0.1);
    vec3 d = vec3 (1,1,1);
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
float map(vec3 position,float traveled){
    traveled=traveled*sqrt(traveled)/200;
    position.xyz = mod(position.xyz,4.0) - 2;
    vec3 P1 = position;
    vec3 P2 = position;
    vec3 P3 = position;



    position.xy+=traveled;
    P1.xz+=traveled;
    P2.zy+=traveled;

    float sphere = sdSphere(position, 0.15);
    float sphere2 = sdSphere(P1, 0.15);
    float sphere3 = sdSphere(P2, 0.15);


    return  smin(smin(sphere,sphere2,0.5),sphere3,0.5);
}

vec3 col = vec3(0);

// MAIN
void main(){
    
    vec2 uv = pos;
    uv.x *= resolution.x/resolution.y;


    vec3 RayOrigin = CamPos;

    vec3 RayDirection =normalize(vec3(uv.x,uv.y,2.0));

    RayDirection=normalize(quaternionRotationMatrix(quaternion)*RayDirection);
   
    
    float traveled =0.0;
    float distance = 0.0;
    // ray behavior
    int i=0;
    for (i = 0; i < 80; i++){
        vec3 position = RayOrigin + RayDirection * traveled;

        distance = map(position,traveled);
        traveled += distance;

        
        if(distance<0.01 || traveled > 1000.) break;
        
    }

if(traveled<900)col = palette(float(i)*0.004);

    FragColor = vec4(col,1.0);
}