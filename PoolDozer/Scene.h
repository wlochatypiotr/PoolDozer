//#pragma once
////#include "World.h"
//
//class CScene
//{
//	CScene(CWorld * world);
//	~CScene();
//	CScene(const CScene&) = delete;
//	CScene& operator=(const CScene&) = delete;
//
//	CEntity* GetEntity(const entity_id_t& id);
//	CEntityComponent* GetComponent(const entity_component_id_t& id);
//	void AddNewEntity(const entity_id_t& id);
//	void AddComponent(const entity_component_id_t& id, EComponentTypeEnum type);
//	void Update();
//	void Draw();
//	//GetEntity
//	//Draw
//	//Update
//	
//private:
//	std::unordered_map <entity_id_t, std::unique_ptr<CEntity> > m_entities;
//	CWorld * m_world;
//};