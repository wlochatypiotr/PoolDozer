#pragma once
#include <string>
#include <map>
#include "EntityComponent.h"

using entityIdType = std::string;
//using componentTableType = std::map<const entityIdType, CEntityComponent*>;

struct Transform {
	int m_position[4];
};

class CEntity
{
public:
	CEntity(const entityIdType& id);

	const Transform& GetTransform() const { return m_transform; }
	void SetTransform(const Transform& xform) { m_transform = xform; }
	const entityIdType& GetID() const { return m_entityID; }
	void SetID(const entityIdType& id) { m_entityID = id; }

	CEntityComponent* GetEntityComponent(const entityIdType& familyID);
	CEntityComponent* SetEntityComponent(CEntityComponent* newEntityComponent);
	void ClearComponents();

private:
	Transform m_transform;
	entityIdType m_entityID;

	using componentTableType = std::map<const CEntityComponent::entityComponentIdType, CEntityComponent*>;

	componentTableType m_components; //map of all the components;
};