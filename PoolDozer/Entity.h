#pragma once
#include "TinyModel.h"

class Entity : public TinyModel
{
public:
	Entity(const char * modelfile
		, ShaderLoader& program
		, glm::mat4 * model
		, glm::mat4  * view
		, glm::mat4 * projection
		, glm::vec3 * color
		, glm::vec3 * lightColor
		, glm::vec3 * lightPosition
		, glm::vec3 position);
};