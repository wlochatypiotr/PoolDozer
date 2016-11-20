#include "Entity.h"

CEntity::CEntity(const entity_id_t & id) : m_entityID(id)
{
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
