#version 430 core

out vec4 color;

in vec3 Normal;
in vec4 fragPos;

layout (location = 3) uniform mat4 model;
layout (location = 4) uniform mat4 view;
layout (location = 5) uniform mat4 projection;

layout (location = 6) uniform vec3 objectColor;
layout (location = 7) uniform vec3 lightColor;
layout (location = 8) uniform vec3 lightPos;
void main()
	{
	//ambient light
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	//diffuse light
	vec4 normalT = projection * view * model * vec4 (Normal, 1.0f) ;
	vec3 norm =  normalize(Normal);
	vec3 lightDir = normalize(lightPos - fragPos.xyz);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0f);
	}