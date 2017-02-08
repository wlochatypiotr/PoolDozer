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
	Load(LAMP_SHADER, Shader("shaders/Phong_VS.frag", "shaders/ColorOnly_FS.frag"));
	Load(TABLE_SHADER, Shader("shaders/Phong_VS.frag", "shaders/Phong_FS.frag"));
}

void CShaderManager::Load(const int& id, Shader shader)
{
	m_map.emplace(id, shader);
}
