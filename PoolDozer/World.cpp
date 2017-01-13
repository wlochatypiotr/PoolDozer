#include "World.h"
CScene::~CScene()
{

}

CScene::CScene(CWorld * world) : m_world(world)
{

}

CEntity * CScene::GetEntity(const entity_id_t & id)
{
	return m_entities.at(id).get();
}

CEntityComponent * CScene::GetComponent(const entity_component_id_t & id)
{
	return m_world->GetComponent(id);
}

void CScene::AddNewEntity(const entity_id_t & id)
{
	std::unique_ptr<CEntity> entity = std::make_unique<CEntity>(id);

	m_entities.emplace(id, std::move(entity));
}

void CScene::AddComponent(const entity_component_id_t& id, EComponentTypeEnum type)
{
	m_world->AddComponent(id, type);
}

void CScene::Update()
{
	using iterator_t = std::unordered_map <entity_id_t, std::unique_ptr<CEntity> >::iterator;
	for (iterator_t iter = m_entities.begin(); iter != m_entities.end(); ++iter)
	{
		iter->second.get()->Update(/*time*/);
	}
}

void CScene::Draw()
{
	using iterator_t = std::unordered_map <entity_id_t, std::unique_ptr<CEntity> >::iterator;
	for (iterator_t iter = m_entities.begin(); iter != m_entities.end(); ++iter)
	{
		m_world->GetRenderer()->Draw(iter->second.get());
	}
}

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
}

void CWorld::Initialize(CComponentManager * manager, CRenderer* renderer)
{
	m_componentManager = manager;
	m_renderer = renderer;
}

CEntity * CWorld::GetEntity(const entity_id_t & id)
{
	return m_entities.at(id).get();
}

CEntityComponent * CWorld::GetComponent(const entity_component_id_t & id)
{
	return m_componentManager->GetComponent(id);
}

CRenderer * CWorld::GetRenderer()
{
	return m_renderer;
}

//constructs CEntity object with id set to id
void CWorld::AddNewEntity(const entity_id_t & id)
{
	std::unique_ptr<CEntity> entity = std::make_unique<CEntity>(id);

	m_entities.emplace(id, std::move(entity));
}
//type 0 = MESH
//type 1 = SPHERE
//type 2 = PHYSX_COMPONENT
void CWorld::AddComponent(const entity_component_id_t & id, EComponentTypeEnum type)
{
	m_componentManager->AddComponent(id, type);
}


CScene * CWorld::AddScene(const scene_id & sceneName)
{
	std::unique_ptr<CScene> scene = std::make_unique<CScene>(this);
	auto ret = scene.get();
	m_levels.emplace(sceneName, std::move(scene));
	return ret;
}

CScene * CWorld::GetScene(const scene_id & sceneName)
{
	return m_levels.at(sceneName).get();
}

void CWorld::Update()
{
	using iterator_t = std::unordered_map <scene_id, std::unique_ptr<CScene> >::iterator;
	for (iterator_t iter = m_levels.begin(); iter != m_levels.end(); ++iter)
	{
		iter->second.get()->Update(/*time*/);
	}
}

void CWorld::Draw()
{
	if (m_renderer != nullptr) {

		m_renderer->ClearBuffer();

		using iterator_t = std::unordered_map <scene_id, std::unique_ptr<CScene> >::iterator;
		for (iterator_t iter = m_levels.begin(); iter != m_levels.end(); ++iter)
		{
			iter->second.get()->Draw();
		}
	}
	else
		std::cout << "No renderer attached to CWorld instance!" << std::endl;
}

void CWorld::SetRenderer(CRenderer* render)
{
	m_renderer = render;
}


