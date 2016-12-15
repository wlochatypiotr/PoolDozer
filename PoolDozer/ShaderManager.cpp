#include "ShaderManager.h"

CShaderManager::CShaderManager()
{
}

Shader& CShaderManager::Get(const int & id)
{
	return m_map[id];
}

void CShaderManager::Load(const int& id, Shader shader)
{
	m_map.emplace(id, shader);
}
