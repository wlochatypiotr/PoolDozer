#pragma once
#include <string>
#include"Entity.h"

class CEntityComponent
{
public:
	using entityComponentIdType = std::string;

	CEntityComponent() : m_owner(nullptr) {};
	virtual ~CEntityComponent() = 0 {};

	virtual const entityComponentIdType& ComponentID() const = 0;
	virtual const entityComponentIdType& FamilyID() const = 0;

	virtual void Update() {};

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
	virtual const entityComponentIdType& FamilyID() const
	{
		return entityComponentIdType("ECVisual");
	}
	virtual void Render() const = 0;
};

//Spherical entity visual component
class CECVisualSphere : public CECVisual
{
public:
	virtual const entityComponentIdType& FamilyID() const
	{
		return entityComponentIdType("ECVisualSphere");
	}

	virtual void Render() const;

	CECVisualSphere(float radius);

	const float GetRadius() const { return m_radius; }
	void SetRadius(const float r) { m_radius = r; }

private:
	float m_radius;
};