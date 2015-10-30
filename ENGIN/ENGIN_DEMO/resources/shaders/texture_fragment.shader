#version 120

varying vec4 color;
varying vec2 texCoords;

uniform sampler2D ourTexture;

void main()
{
	gl_FragColor = color * texture2D(ourTexture, texCoords);
}