#pragma once
#include <unordered_map>
#include "ShaderLoader.h"

class CShaderManager
{
public:
	enum EShaderID
	{
		LAMP_SHADER,
		TABLE_SHADER
	};

	CShaderManager(const CShaderManager&) = delete;
	CShaderManager& operator=(const CShaderManager&) = delete;
	CShaderManager();

	Shader& Get(const int& id);
	void Initialize();
	void Load(const int& id, Shader shader);
private:
	std::unordered_map <int, Shader > m_map;

};
