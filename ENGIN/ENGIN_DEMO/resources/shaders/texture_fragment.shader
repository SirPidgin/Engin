#version 120

varying vec4 color;
varying vec2 texCoords;

uniform sampler2D ourTexture;

void main()
{
	vec4 texel = texture2D(ourTexture, texCoords);
	if (texel.a < 1.0f)
		discard;
	gl_FragColor = color * texel;
}