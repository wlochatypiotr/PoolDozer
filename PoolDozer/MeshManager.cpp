#include "MeshManager.h"

CMeshManager::CMeshManager() 
{

}

CMeshManager::~CMeshManager()
{

}

//will throw exception if no such element exists
CMesh * CMeshManager::Get(mesh_id_t id)
{
	return m_map.at(id).get();
}

bool CMeshManager::Load(mesh_id_t id, mesh_id_t filename)
{
	std::unique_ptr<CMesh> pmesh = std::make_unique<CMesh>();
	bool ret = pmesh->LoadMesh(filename);
	if (ret)
		printf("Mesh %s loaded successfully\n", filename.c_str());
	else
		printf("Mesh %s loading failed\n", filename.c_str());

	m_map.emplace(id, std::move(pmesh));

	return ret;
}

void CMeshManager::ShutDown()
{
	Clear();
}

void CMeshManager::Clear()
{
	m_map.clear();
}

void CMeshManager::Initialize()
{
	Load("MCube", "models/Cube.obj");
	Load("MTable", "models/PoolTable.obj");
}
