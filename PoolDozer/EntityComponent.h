#pragma once
#include <string>
#include <vector>
//#include"Entity.h"
//for rendering data 
#include "TinyModel.h"
//#include "Renderer.h"

using entity_component_id_t = std::string;
class CEntity;

class CEntityComponent
{
public:
	CEntityComponent() : m_owner(nullptr) {}
	virtual ~CEntityComponent() = 0 {}

	virtual const entity_component_id_t& ComponentID() const = 0;
	virtual const entity_component_id_t& FamilyID() const = 0;

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
	virtual const entity_component_id_t& FamilyID() const
	{
		return entity_component_id_t("ECVisual");
	}
	virtual void Render() const = 0;

private:
	void SetColor(const float& r, const float& g, const float& b, const float& a);
	float m_color[4];

};

//Spherical entity visual component
class CECVisualSphere : public CECVisual
{
public:
	virtual const entity_component_id_t& ComponentID() const
	{
		return entity_component_id_t("ECVisualSphere");
	}

	virtual void Render() const;

	CECVisualSphere(float radius);

	const float GetRadius() const { return m_radius; }
	void SetRadius(const float r) { m_radius = r; }

private:
	float m_radius;
};

class CECVisualMesh : public CECVisual
{
public:
	virtual const entity_component_id_t& ComponentID() const
	{
		return entity_component_id_t("ECVisualMesh");
	}
	virtual void Render() const;

	//CECVisualMesh(const std::vector<RenderingData> &mesh);
	void SetMesh(const std::vector<RenderingData> &mesh);

private:
	std::vector<RenderingData> m_mesh;
};