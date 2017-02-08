#version 430

layout (location = 0) in vec3 in_vertexPos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_UV;

layout (location = 3) uniform mat4 u_model;
layout (location = 4) uniform mat4 u_view;
layout (location = 5) uniform mat4 u_projection;

out vec3 o_normal;
out vec4 o_fragPos;
void main()
	{
		gl_Position = u_projection * u_view * u_model * vec4 (in_vertexPos, 1.0f);
		o_fragPos = vec4(u_model * vec4(in_vertexPos, 1.0f));
		o_normal = in_normal;
	}