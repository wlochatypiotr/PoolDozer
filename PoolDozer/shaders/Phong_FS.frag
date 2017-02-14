#version 430 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	};

out vec4 color;

in vec3 v_Normal;
in vec4 v_fragPos;
in vec2 v_TexCoord;

layout (location = 3) uniform mat4 u_model;
layout (location = 4) uniform mat4 u_view;
layout (location = 5) uniform mat4 u_projection;

layout (location = 6) uniform vec3 u_objectColor;
layout (location = 7) uniform vec3 u_lightColor;
layout (location = 8) uniform vec3 u_lightPos;
layout (location = 9) uniform vec3 u_cameraPosition;

uniform Material u_material;
uniform Light u_light;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
void main()
	{
	//ambient light
    vec3 ambient = u_light.ambient * u_material.ambient;

	//diffuse light
	vec3 norm =  normalize(v_Normal);
	vec3 lightDir = normalize(u_light.position - v_fragPos.xyz);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =  u_light.diffuse * (diff * u_material.diffuse);

	//specular
	vec3 viewDir = normalize(u_cameraPosition - v_fragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	vec3 specular = u_light.specular * (spec * u_material.specular);

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
	//color = texture(texture_diffuse, v_TexCoord);
	}