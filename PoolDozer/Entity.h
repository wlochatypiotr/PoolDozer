#pragma once
#include "EntityComponent.h"
#include <map>


using namespace glm;
class CRenderer;
using entity_id_t = std::string;


struct Transform {
	vec3 m_position;
	vec3 m_rotation; // x, y, z rotations
	vec3 m_scale;
	Transform() 
	{
	}
	Transform(const Transform & rhs)
	{
		this->m_position = rhs.m_position;
		this->m_rotation = rhs.m_rotation;
		this->m_scale = rhs.m_scale;
	}
};

class CEntity
{
	friend class CScene;
public:
	using entity_id_t = std::string;
	CEntity(const entity_id_t& id, bool isVisible = true ,bool isActive = true);
	//hoe copy c should work ?!
	CEntity(const CEntity & rhs);

	const Transform& GetTransform() const;
	void SetTransform(const Transform& xform);
	void SetPosition(const vec3& pos);
	void SetRotation(const vec3& rot);
	void SetRotation(const float& x, const float& y, const float& z);
	void SetScale(const vec3& scale);
	void SetScale(const float& uniformScale);
	const entity_id_t& GetID() const;
	void SetID(const entity_id_t& id);

	bool IsVisible() { return m_isVisible; }
	void SetVisible(bool flag) { m_isVisible = flag; }

	//void SetActive(bool activationState);

	bool IsActive();
	void Activate() { m_isActive = true; }
	void Deactivate() { m_isActive = false; }

	//This function will update all Entity components
	//TODO: 
	//add time as argument for use of some components
	void Update();

	CEntityComponent* GetEntityComponent(const entity_component_id_t& familyID);
	CEntityComponent* SetEntityComponent(CEntityComponent* newEntityComponent);
	void ClearComponents();

private:
	Transform m_transform;
	entity_id_t m_entityID;
	bool m_isVisible;
	bool m_isActive;

	//for CEntity copy constructor
	static int m_counter;

	using componentTableType = std::map<const entity_component_id_t, CEntityComponent* >;
	componentTableType m_components; //map of all the components;
};
