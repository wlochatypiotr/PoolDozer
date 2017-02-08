#version 430 core
precision mediump float;
uniform sampler2D u_Sampler;
in vec2 v_TexCoordinates;
out vec4 v_FragColor;
const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);

void main() 
	{
	vec4 c = texture(u_Sampler, v_TexCoordinates);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	v_FragColor = vec4 (c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y), 1.0);
	}