//#include "Scene.h"
//#include "World.h"
//
//CScene::~CScene()
//{
//
//}
//
//CScene::CScene(CWorld * world) : m_world(world)
//{
//
//}
//
//CEntity * CScene::GetEntity(const entity_id_t & id)
//{
//	return m_entities.at(id).get();
//}
//
//CEntityComponent * CScene::GetComponent(const entity_component_id_t & id)
//{
//	return m_world->GetComponent(id);
//}
//
//void CScene::AddNewEntity(const entity_id_t & id)
//{
//	std::unique_ptr<CEntity> entity = std::make_unique<CEntity>(id);
//
//	m_entities.emplace(id, std::move(entity));
//}
//
//void CScene::AddComponent(const entity_component_id_t& id, EComponentTypeEnum type)
//{
//	m_world->AddComponent(id, type);
//}
//
//void CScene::Update()
//{
//	using iterator_t = std::unordered_map <entity_id_t, std::unique_ptr<CEntity> >::iterator;
//	for (iterator_t iter = m_entities.begin(); iter != m_entities.end(); ++iter)
//	{
//		iter->second.get()->Update(/*time*/);
//	}
//}
//
//void CScene::Draw()
//{
//	using iterator_t = std::unordered_map <entity_id_t, std::unique_ptr<CEntity> >::iterator;
//	for (iterator_t iter = m_entities.begin(); iter != m_entities.end(); ++iter)
//	{
//		m_world->GetRenderer()->Draw(iter->second.get());
//	}
//}
