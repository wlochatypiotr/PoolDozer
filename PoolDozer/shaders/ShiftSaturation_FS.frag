#version 430 core

precision mediump float;
uniform sampler2D u_Sampler;
in vec2 v_TexCoordinates;
out vec4 v_FragColor;

void main() 
	{
    vec4 color = texture(u_Sampler, v_TexCoordinates);
	if(color.r > 75.0/360.0 && color.r < 170.0 / 360.0)
		color.g -= 0.5;
	clamp(color.g, 0.0, 1.0);
    v_FragColor = color;
    }