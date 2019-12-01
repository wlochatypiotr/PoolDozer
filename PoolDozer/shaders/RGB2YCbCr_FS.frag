#version 430 core

precision mediump float;
uniform sampler2D u_Sampler;
const mat4 YUV = mat4(0.299, 0.587, 0.114, 0.0,
                     -0.169,-0.331, 0.500, 0.0,
                      0.500,-0.419,-0.081, 0.0,
                      0.000, 0.500, 0.500, 1.00);
in vec2 v_TexCoordinates;
out vec4 v_FragColor;

void main() 
	{
	v_FragColor = YUV * texture(u_Sampler, v_TexCoordinates) ;
	}