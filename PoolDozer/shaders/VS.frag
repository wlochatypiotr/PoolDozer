#version 430

layout (location = 0) in vec3 vertices;
layout (location = 1) in vec3 normal;


layout (location = 3) uniform mat4 model;
layout (location = 4) uniform mat4 view;
layout (location = 5) uniform mat4 projection;

out vec3 Normal;
out vec4 fragPos;
void main()
	{
		fragPos = projection * view * model * vec4 (vertices, 1.0f);
		gl_Position = fragPos;
		Normal = normal;
	}