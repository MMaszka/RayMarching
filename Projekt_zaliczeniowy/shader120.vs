#version 120
	
attribute vec3 aPos;

varying vec2 pos;


void main()
{
	pos = aPos.xy;
	gl_Position = vec4(aPos,1.0);
}