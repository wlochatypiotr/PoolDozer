#version 430

layout (location = 0) in vec3 vertices;



layout (location = 3) uniform mat4 model;
layout (location = 4) uniform mat4 view;
layout (location = 5) uniform mat4 projection;

void main()
	{
		vec4 fragPos;
		fragPos = projection * view * model * vec4 (vertices, 1.0f);
		gl_Position = fragPos;
	}