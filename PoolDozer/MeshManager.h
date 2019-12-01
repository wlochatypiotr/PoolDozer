#pragma once
#include "Model.h"
#include <unordered_map>
#include <memory>

using model_id_t = std::string;

class CModelManager
{
public:
	CModelManager(const CModelManager&) = delete;
	CModelManager& operator=(const CModelManager&) = delete;
	CModelManager();
	~CModelManager();

	CModel* Get(model_id_t id);
	bool Load(model_id_t id, model_id_t filename);
	void ShutDown();
	void Clear();
	void Initialize();
private:
	std::unordered_map <model_id_t, std::unique_ptr<CModel> > m_map;
};
