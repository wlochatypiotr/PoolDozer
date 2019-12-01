#include "ComponentManager.h"



CComponentManager::CComponentManager()
{
}


CComponentManager::~CComponentManager()
{
}

void CComponentManager::AddComponent(const entity_component_id_t & id, EComponentTypeEnum type)
{
	std::unique_ptr<CEntityComponent> component;// = std::make_unique<CECVisualMesh>();
												//m_components.emplace(id, std::move(component));

	switch (type)
	{
	case MESH_COMPONENT:
		component = std::make_unique<CECVisualMesh>();
		m_components.emplace(id, std::move(component));
		break;
		/*case SPHERE_COMPONENT:
		component = std::make_unique<CECVisualSphere>();
		m_components.emplace(id, std::move(component));
		break;*/
	default:
		std::cout << "Wrong component type" << std::endl;
	}
}

CEntityComponent * CComponentManager::GetComponent(const entity_component_id_t & id)
{
	return m_components.at(id).get();
}
