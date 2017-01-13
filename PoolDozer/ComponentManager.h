#pragma once
#include <unordered_map>
#include <memory>
#include "EntityComponent.h"

class CComponentManager
{
public:
	CComponentManager();
	~CComponentManager();
	CComponentManager(const CComponentManager&) = delete;
	CComponentManager& operator=(const CComponentManager&) = delete;

	void Initialize();
	void AddComponent(const entity_component_id_t& id, EComponentTypeEnum type);
	CEntityComponent* GetComponent(const entity_component_id_t& id);

private:
	std::unordered_map <entity_component_id_t, std::unique_ptr<CEntityComponent> > m_components;
};

