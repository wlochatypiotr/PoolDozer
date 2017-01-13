#pragma once
#include "Mesh.h"
#include <unordered_map>
#include <memory>

using mesh_id_t = std::string;

class CMeshManager
{
public:
	CMeshManager(const CMeshManager&) = delete;
	CMeshManager& operator=(const CMeshManager&) = delete;
	CMeshManager();
	~CMeshManager();

	CMesh* Get(mesh_id_t id);
	bool Load(mesh_id_t id, mesh_id_t filename);
	void ShutDown();
	void Clear();
	void Initialize();
private:
	std::unordered_map <mesh_id_t, std::unique_ptr<CMesh> > m_map;
};
