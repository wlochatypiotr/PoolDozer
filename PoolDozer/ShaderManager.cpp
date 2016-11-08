#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

Shader& ShaderManager::Get(const int & id)
{
	return m_map[id];
}

void ShaderManager::Load(const int& id, Shader shader)
{
	m_map.emplace(id, shader);
}
