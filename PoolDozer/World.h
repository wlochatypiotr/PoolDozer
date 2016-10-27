#pragma once
#include <list>
#include "TinyModel.h"

class Configuration;

class World
{
public:
	World();
	World(const World&) = delete;
	World& operator= (const World&) = delete;
	void AddEntity(TinyModel* entity);
	void Update(GLfloat deltaTime);
	void Draw();
	void ProcessInput();

	std::list<TinyModel*> m_entities;
	glm::vec3 m_playerPosition;
	glm::vec3 m_boardPosition;
	glm::vec3 m_lightPositon;
};