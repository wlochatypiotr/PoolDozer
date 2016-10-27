#include "World.h"

World::World()
{
};

void World::AddEntity(TinyModel * entity)
{
	m_entities.push_back(entity);
}

void World::Draw()
{
	for (auto c : m_entities)
		c->Draw();
}
