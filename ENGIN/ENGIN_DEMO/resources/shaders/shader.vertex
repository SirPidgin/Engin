#version 120

attribute vec3 inPosition;
attribute vec4 inColor;

varying vec4 color;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(inPosition, 1);
	color = inColor;
}