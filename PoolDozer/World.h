#pragma once
#include "Renderer.h"
class CWorld
{
public:
	enum EComponentTypeEnum {
		MESH_COMPONENT = 0,
		SPHERE_COMPONENT = 1,
		PHYSIC_COMPONET = 2
	};

	CWorld(const CWorld&) = delete;
	CWorld& operator=(const CWorld&) = delete;
	CWorld(Renderer* rend = nullptr) : m_renderer(rend) {};

	CEntity* GetEntity(const entity_id_t& id);
	CEntityComponent* GetComponent(const entity_component_id_t& id);
	void AddNewEntity(const entity_id_t& id);
	void AddComponent(const entity_component_id_t& id, EComponentTypeEnum type);
	void Update();
	void Draw();
	void SetRenderer(Renderer* render);
private:
	std::unordered_map <entity_id_t, std::unique_ptr<CEntity> > m_entities;
	std::unordered_map <entity_component_id_t, std::unique_ptr<CEntityComponent> > m_components;

	Renderer* m_renderer;
};

CEntity * CWorld::GetEntity(const entity_id_t & id)
{
	return m_entities.at(id).get();
}

inline CEntityComponent * CWorld::GetComponent(const entity_component_id_t & id)
{
	return m_components.at(id).get();
}

//constructs CEntity object with id set to id
inline void CWorld::AddNewEntity(const entity_id_t & id)
{
	std::unique_ptr<CEntity> entity = std::make_unique<CEntity>(id);

	m_entities.emplace(id, std::move(entity));
}
//type 0 = MESH
//type 1 = SPHERE
//type 2 = PHYSX_COMPONENT
inline void CWorld::AddComponent(const entity_component_id_t & id, EComponentTypeEnum type)
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

inline void CWorld::Update()
{
	using iterator_t = std::unordered_map <entity_id_t, std::unique_ptr<CEntity> >::iterator;
	for (iterator_t iter = m_entities.begin(); iter != m_entities.end(); ++iter)
	{
		iter->second.get()->Update(/*time*/);
	}
}

inline void CWorld::Draw()
{
	if (m_renderer != nullptr) {

		m_renderer->ClearBuffer();

		using iterator_t = std::unordered_map <entity_id_t, std::unique_ptr<CEntity> >::iterator;
		for (iterator_t iter = m_entities.begin(); iter != m_entities.end(); ++iter)
		{
			m_renderer->Draw(iter->second.get());
		}
	}
	else
		std::cout << "No renderer attached to CWorld instance!" << std::endl;
}

inline void CWorld::SetRenderer(Renderer* render)
{
	m_renderer = render;
}
