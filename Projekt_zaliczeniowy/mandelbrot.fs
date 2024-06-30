#version 120

uniform vec2 resolution;
uniform float iTime;
uniform vec3 CamPos;
uniform float velocity;

uniform vec4 quaternion;
uniform vec2 mPos;

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
    vec3 b = vec3 (0.2,0.7,0.7);
    vec3 c = vec3 (0.2,0.8,0.9);
    vec3 d = vec3 (0.163,0.3,0.7257);
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

vec2 powComplex(vec2 C,vec2 p){
    return vec2(C.x*C.x-C.y*C.y+p.x,2*C.x*C.y+p.y);
}

void main(){
    
    vec2 mpos = vec2(mPos)/resolution.y*2-2;

    vec2 p = mpos;
    p.y+=1;

    vec2 uv = pos*1.2/(0.01+pow(iTime,iTime/16))-0.152697;
    uv.x *= resolution.x/(resolution.y);
    uv.x-=0.5;
    p=uv;
    int i = 0;

    vec2 C = vec2(uv.x,uv.y);

    while(i<50&&length(C)<200){
        C = powComplex(C,p);
        i++;
    }

    vec3 col = vec3(pow(1-float(i)/50,1)/2,pow(1-float(i)/50,1)/2,pow(1-float(i)/50,1));
    FragColor = vec4(col,1.0);
}