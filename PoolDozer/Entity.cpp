#include "Entity.h"

CEntity::CEntity(const entity_id_t & id) : m_entityID(id)
{
}

const Transform& CEntity::GetTransform() const
{
	return m_transform;
}

void CEntity::SetTransform(const Transform& xform)
{
	m_transform = xform;
}

void CEntity::SetPosition(const vec3& pos)
{
	m_transform.m_position = pos;
}

void CEntity::SetRotation(const vec3 & rot)
{
	m_transform.m_rotation = rot;
}

void CEntity::SetRotation(const float& x, const float& y, const float& z)
{
	m_transform.m_rotation.x = x;
	m_transform.m_rotation.y = y;
	m_transform.m_rotation.z = z;
}

void CEntity::SetScale(const vec3& scale)
{
	m_transform.m_scale = scale;
}

void CEntity::SetScale(const float& uniformScale)
{
	m_transform.m_scale.x = uniformScale;
	m_transform.m_scale.y = uniformScale;
	m_transform.m_scale.z = uniformScale;
}

const entity_id_t& CEntity::GetID() const
{
	return m_entityID;
}

void CEntity::SetID(const entity_id_t& id)
{
	m_entityID = id;
}

void CEntity::Update()
{
	using iterator_t = std::map<const entity_component_id_t, CEntityComponent* >::iterator;
	for (iterator_t it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->Update();
	}
}

//returns nullptr if there is no such component
CEntityComponent * CEntity::GetEntityComponent(const entity_component_id_t & familyID)
{
	using it = std::map<entity_component_id_t, CEntityComponent*>::iterator;
	it findResult = m_components.find(familyID);
	if (findResult == m_components.end())
		return nullptr;
	else
		return findResult->second;
}

//set component in entity to passed component, if such component already exists
//replace it with new component and return pointer to replaced component
CEntityComponent * CEntity::SetEntityComponent(CEntityComponent * newEntityComponent)
{
	using it = std::map<entity_component_id_t, CEntityComponent*>::iterator;
	it findResult = m_components.find(newEntityComponent->FamilyID());

	if (findResult != m_components.end())
	{
		CEntityComponent* previous = findResult->second;
		previous->SetOwningEntity(nullptr);
		newEntityComponent->SetOwningEntity(this);
		findResult->second = newEntityComponent;
		return previous;
	}
	else
	{
		newEntityComponent->SetOwningEntity(this);
		auto ret = m_components.emplace(std::make_pair(newEntityComponent->FamilyID(), newEntityComponent));
		return ret.first->second;
	}
}

void CEntity::ClearComponents()
{
	m_components.clear();
}
