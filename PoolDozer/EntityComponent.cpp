#include "EntityComponent.h"

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


void CECVisualMesh::SetMesh(const std::vector<RenderingData>& mesh)
{
	m_mesh = mesh;
}
