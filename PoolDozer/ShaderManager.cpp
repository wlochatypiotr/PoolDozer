#include "ShaderManager.h"

CShaderManager::CShaderManager()
{
}

Shader& CShaderManager::Get(const int & id)
{
	return m_map[id];
}

void CShaderManager::Initialize()
{
	Load(LAMP_SHADER, Shader("shaders/VS.frag", "shaders/LampShader.frag"));
	Load(TABLE_SHADER, Shader("shaders/VS.frag", "shaders/FS.frag"));
}

void CShaderManager::Load(const int& id, Shader shader)
{
	m_map.emplace(id, shader);
}
