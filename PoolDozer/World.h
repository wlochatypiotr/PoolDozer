#pragma once
#include "ComponentManager.h"
#include "Entity.h"
#include <unordered_map>
#include <memory>

using scene_id = std::string;
class CWorld;

class CScene
{
public:

	CScene(CWorld * world);
	~CScene();
	CScene(const CScene&) = delete;
	CScene& operator=(const CScene&) = delete;

	CEntity* GetEntity(const entity_id_t& id);
	CEntityComponent* GetComponent(const entity_component_id_t& id);
	void AddNewEntity(const entity_id_t& id);
	void AddNewEntity(const entity_id_t& id, CEntity * copy);
	void AddComponent(const entity_component_id_t& id, EComponentTypeEnum type);
	void SetActive(bool activationState);
	bool IsActive();
	const std::unordered_map <entity_id_t, std::unique_ptr<CEntity> > & GetEntities()const;
	void Update();

private:
	std::unordered_map <entity_id_t, std::unique_ptr<CEntity> > m_entities;
	CWorld * m_world;
	bool m_isActive;
};

class CWorld
{
public:
	CWorld();
	~CWorld();
	CWorld(const CWorld&) = delete;
	CWorld& operator=(const CWorld&) = delete;

	void Initialize(CComponentManager* manager);

	CEntity* GetEntity(const entity_id_t& id);
	CEntityComponent* GetComponent(const entity_component_id_t& id);

	void AddNewEntity(const entity_id_t& id);
	void AddComponent(const entity_component_id_t& id, EComponentTypeEnum type);
	CScene * AddScene(const scene_id & sceneName, bool isActive = true);
	CScene * GetScene(const scene_id & sceneName);
	const std::unordered_map <scene_id, std::unique_ptr<CScene> >& GetLevels() const;
	void Update();

private:
	std::unordered_map <entity_id_t, std::unique_ptr<CEntity> > m_entities;
	std::unordered_map <scene_id, std::unique_ptr<CScene> > m_levels;
	CComponentManager* m_componentManager;
};

