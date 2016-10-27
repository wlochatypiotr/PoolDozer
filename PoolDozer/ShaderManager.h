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

	ShaderLoader& Get(const int& id);
	void Load(const int& id, ShaderLoader shader);
private:
	std::unordered_map <int, ShaderLoader > m_map;
};
