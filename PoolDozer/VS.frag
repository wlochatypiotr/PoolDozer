#version 430

layout (location = 0) in vec3 vertices;
layout (location = 4) in vec2 texCord;

layout (location = 1) uniform mat4 model;
layout (location = 2) uniform mat4 view;
layout (location = 3) uniform mat4 projection;

out vec2 tc;
void main()
	{
		gl_Position = projection * view * model * vec4(vertices, 1.0f);
		tc = texCord;
	}