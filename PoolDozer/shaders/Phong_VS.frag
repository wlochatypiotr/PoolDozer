#version 430

layout (location = 0) in vec3 v_vertexPos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_UV;

layout (location = 3) uniform mat4 u_model;
layout (location = 4) uniform mat4 u_view;
layout (location = 5) uniform mat4 u_projection;

out vec3 v_Normal;
out vec4 v_fragPos;
out vec2 v_TexCoord;
void main()
	{
		gl_Position = u_projection * u_view * u_model * vec4 (v_vertexPos, 1.0f);
		v_fragPos = vec4(u_model * vec4(v_vertexPos, 1.0f));
		v_Normal = v_normal;
		v_TexCoord = v_UV;
	}