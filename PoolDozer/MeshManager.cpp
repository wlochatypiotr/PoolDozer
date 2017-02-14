#include "MeshManager.h"

CModelManager::CModelManager() 
{

}

CModelManager::~CModelManager()
{

}

//will throw exception if no such element exists
CModel * CModelManager::Get(model_id_t id)
{
	return m_map.at(id).get();
}

bool CModelManager::Load(model_id_t id, model_id_t filename)
{
	std::unique_ptr<CModel> pmesh = std::make_unique<CModel>();
	bool ret = pmesh->LoadModel(filename);
	if (ret)
		printf("Mesh %s loaded successfully\n", filename.c_str());
	else
		printf("Mesh %s loading failed\n", filename.c_str());

	m_map.emplace(id, std::move(pmesh));

	return ret;
}

void CModelManager::ShutDown()
{
	Clear();
}

void CModelManager::Clear()
{
	m_map.clear();
}

void CModelManager::Initialize()
{
	Load("mesh_cube", "models/Cube.obj");
	Load("mesh_table", "models/PoolTable.obj");
	Load("mesh_dozer", "models/nanosuit.obj");
}
