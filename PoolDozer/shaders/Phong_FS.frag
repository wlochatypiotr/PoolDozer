#version 430 core

out vec4 color;

in vec3 o_normal;
in vec4 o_fragPos;

layout (location = 3) uniform mat4 u_model;
layout (location = 4) uniform mat4 u_view;
layout (location = 5) uniform mat4 u_projection;

layout (location = 6) uniform vec3 u_objectColor;
layout (location = 7) uniform vec3 u_lightColor;
layout (location = 8) uniform vec3 u_lightPos;
layout (location = 9) uniform vec3 u_cameraPosition;
void main()
	{
	//ambient light
	float ambientStrength = 0.1;
	float specularStrength = 0.5;
    vec3 ambient = ambientStrength * u_lightColor;

	//diffuse light
	vec3 norm =  normalize(o_normal);
	vec3 lightDir = normalize(u_lightPos - o_fragPos.xyz);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColor;

	//specular
	vec3 viewDir = normalize(u_cameraPosition - o_fragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    color = vec4(result, 1.0);
	}