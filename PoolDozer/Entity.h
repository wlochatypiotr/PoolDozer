#pragma once
#include <string>
#include <map>
#include <memory>
//#include "Renderer.h"
#include "EntityComponent.h"

class Renderer;
using entity_id_t = std::string;
//using componentTableType = std::map<const entity_id_t, CEntityComponent*>;

struct Transform {
	glm::vec3 m_position;
	glm::vec3 m_rotation; // x, y, z rotations
	glm::vec3 m_scale;
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