#version 430

layout (location = 0) in vec3 vertices;
layout (location = 1) uniform mat4 model;
layout (location = 2) uniform mat4 view;
layout (location = 3) uniform mat4 projection;

out vec4 pos;

void main()
	{
		pos = projection * view * model * vec4 (vertices, 1.0f) ;
		gl_Position =  projection * view * model * vec4 (vertices, 1.0f) ;

	}