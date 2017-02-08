#version 430 core

precision mediump float;
uniform sampler2D u_Sampler;
const mat3 RGB = mat3(1.00, 0.000, 1.400,
                      1.00,-0.343,-0.711,
                      1.00, 1.765, 0.000);
const vec3 ADJ = vec3(0.00, 0.5, 0.5);
in vec2 v_TexCoordinates;
out vec4 v_FragColor;

void main() 
	{
	v_FragColor = vec4(RGB * (texture(u_Sampler, v_TexCoordinates).rgb - ADJ), 1.0);
	}