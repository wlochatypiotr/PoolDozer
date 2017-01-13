#pragma once
#include "Renderer.h"
#include "ComponentManager.h"
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
	void AddComponent(const entity_component_id_t& id, EComponentTypeEnum type);
	void Update();
	void Draw();
private:
	std::unordered_map <entity_id_t, std::unique_ptr<CEntity> > m_entities;
	CWorld * m_world;
};

class CWorld
{
public:
	CWorld();
	~CWorld();
	CWorld(const CWorld&) = delete;
	CWorld& operator=(const CWorld&) = delete;

	void Initialize(CComponentManager* manager, CRenderer * renderer);

	CEntity* GetEntity(const entity_id_t& id);
	CEntityComponent* GetComponent(const entity_component_id_t& id);
	CRenderer * GetRenderer();
	void AddNewEntity(const entity_id_t& id);
	void AddComponent(const entity_component_id_t& id, EComponentTypeEnum type);
	CScene * AddScene(const scene_id & sceneName);
	CScene * GetScene(const scene_id & sceneName);
	void Update();
	void Draw();
	void SetRenderer(CRenderer* render);
private:
	std::unordered_map <entity_id_t, std::unique_ptr<CEntity> > m_entities;
	std::unordered_map <scene_id, std::unique_ptr<CScene> > m_levels;
	CRenderer* m_renderer;
	CComponentManager* m_componentManager;
};

