#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	//vec3 ambient;
	//vec3 diffuse;
	vec3 specular;
	float shininess;
	};

struct Light {
	vec3 position;

	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	};

out vec4 color;

in vec3 v_Normal;
in vec4 v_fragPos;
in vec2 v_TexCoord;

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
	vec4 mat_diffuse = texture(u_material.texture_diffuse1, v_TexCoord);
	vec4 mat_specular = 3.0 *texture(u_material.texture_specular1, v_TexCoord);
    vec3 ambient = u_light.ambient * mat_diffuse.rgb;

	//diffuse light
	vec3 norm =  normalize(v_Normal);
	vec3 lightDir = normalize(-u_light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =  u_light.diffuse * (diff * mat_diffuse.rgb);

	//specular
	vec3 viewDir = normalize(u_cameraPosition - v_fragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	vec3 specular = u_light.specular * (spec * mat_specular.rgb);

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0) * 4;
	//color = texture(u_material.texture_diffuse1, v_TexCoord) ;
	}