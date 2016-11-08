#pragma once
#pragma once
#include <unordered_map>
#include "ShaderLoader.h"

class ShaderManager
{
public:
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager();

	Shader& Get(const int& id);
	void Load(const int& id, Shader shader);
private:
	std::unordered_map <int, Shader > m_map;
};
