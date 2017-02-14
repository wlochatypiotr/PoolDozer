#include "EntityComponent.h"
#include "Entity.h"

void CECVisual::Update()
{
	CEntity * owner = GetOwner();
	Transform trans = owner->GetTransform();
	//update world-model matrix 
	// scale -> translate -> rotate
	m_model = glm::mat4();

	// scale 
	m_model = glm::scale(m_model, trans.m_scale);
	
	//rotation
	if (trans.m_rotation.x != 0)
	{
		m_model = glm::rotate(m_model, glm::radians(trans.m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (trans.m_rotation.y != 0)
	{
		m_model = glm::rotate(m_model, glm::radians(trans.m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (trans.m_rotation.z != 0)
	{
		m_model = glm::rotate(m_model, glm::radians(trans.m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	//translate
	m_model = glm::translate(m_model, trans.m_position);
}

void CECVisual::SetColor(const float & r, const float & g, const float & b)
{
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;

}

void CECVisual::SetProgram(Shader &program)
{
	m_program = &program;
}


void CECVisualMesh::SetMesh(CModel* meshptr)
{

	mp_mesh = meshptr;
	
}
CModel * CECVisualMesh::GetModel()
{
	return mp_mesh;
}

