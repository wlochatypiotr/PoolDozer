#pragma once
#include <string>
#include <map>
#include <memory>
#include "EntityComponent.h"

//using entityIdType = std::string;
//using componentTableType = std::map<const entityIdType, CEntityComponent*>;

struct Transform {
	int m_position[4];
};

class CEntity
{
public:
	using entity_id_t = std::string;
	CEntity(const entity_id_t& id);

	const Transform& GetTransform() const { return m_transform; }
	void SetTransform(const Transform& xform) { m_transform = xform; }
	const entity_id_t& GetID() const { return m_entityID; }
	void SetID(const entity_id_t& id) { m_entityID = id; }

	CEntityComponent* GetEntityComponent(const entity_component_id_t& familyID);
	CEntityComponent* SetEntityComponent(CEntityComponent* newEntityComponent);
	void ClearComponents();

private:
	Transform m_transform;
	entity_id_t m_entityID;

	using componentTableType = std::map<const entity_component_id_t, CEntityComponent* >;

	componentTableType m_components; //map of all the components;
};