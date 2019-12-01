#pragma once
#include "Model.h"
#include "ShaderLoader.h"
#include "glm/gtc/matrix_transform.hpp"

using entity_component_id_t = std::string;
class CEntity;

enum EComponentTypeEnum {
	MESH_COMPONENT = 0,
	SPHERE_COMPONENT = 1,
	PHYSIC_COMPONET = 2
};

class CEntityComponent
{
public:
	CEntityComponent() : m_owner(nullptr) {}
	virtual ~CEntityComponent() =0  {}

	virtual const entity_component_id_t ComponentID() const = 0;
	virtual const entity_component_id_t FamilyID() const = 0;

	virtual void Update() {};

	//what if multiple objects have same component ?
	void SetOwningEntity(CEntity* entity) { m_owner = entity; }
	CEntity* GetOwner() const { return m_owner; }

private:
	CEntity* m_owner; //Entity of which this component is member of
};

//Base class for visual Entity Components
class CECVisual : public CEntityComponent
{
	//common interface
public:
	//virtual ~CECVisual() {}
	virtual const entity_component_id_t FamilyID() const
	{
		return entity_component_id_t("ECVisual");
	}
	virtual void Update() override;


	void SetColor(const float& r, const float& g, const float& b);
	void SetProgram( Shader& program);
	glm::vec3 m_color;
	glm::mat4 m_model;
	Shader * m_program = nullptr;

};

//Spherical entity visual component
class CECVisualSphere : public CECVisual
{
public:
	virtual const entity_component_id_t ComponentID() const
	{
		return entity_component_id_t("ECVisualSphere");
	}

	CECVisualSphere(/*float radius*/);

	const float GetRadius() const { return m_radius; }
	void SetRadius(const float r) { m_radius = r; }

private:
	float m_radius;
};

class CECVisualMesh : public CECVisual
{
public:
	virtual const entity_component_id_t ComponentID() const
	{
		return entity_component_id_t("ECVisualMesh");
	}

	void SetMesh(CModel* meshptr);
	CModel* GetModel();
private:
	CModel * mp_mesh;
};