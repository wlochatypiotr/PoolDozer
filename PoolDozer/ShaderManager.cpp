#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderLoader& ShaderManager::Get(const int & id)
{
	return m_map[id];
}

void ShaderManager::Load(const int& id, ShaderLoader shader)
{
	m_map.emplace(id, shader);
}
