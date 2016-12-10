#pragma once
#include "Mesh.h"
#include <unordered_map>
#include <memory>

using mesh_id_t = std::string;

class MeshManager
{
public:
	MeshManager(const MeshManager&) = delete;
	MeshManager& operator=(const MeshManager&) = delete;
	MeshManager();

	CMesh* Get(mesh_id_t id);
	bool Load(mesh_id_t id, mesh_id_t filename);
private:
	std::unordered_map <mesh_id_t, std::unique_ptr<CMesh> > m_map;
};

MeshManager::MeshManager() {};

//will throw exception if no such element exists
CMesh * MeshManager::Get(mesh_id_t id)
{
	return m_map.at(id).get(); 
}

bool MeshManager::Load(mesh_id_t id, mesh_id_t filename)
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