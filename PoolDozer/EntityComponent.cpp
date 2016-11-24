#include "EntityComponent.h"

void CECVisual::SetColor(const float & r, const float & g, const float & b, const float & a)
{
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
	m_color[3] = a;
}

void CECVisualMesh::Render() const
{
}

void CECVisualMesh::SetMesh(const std::vector<RenderingData>& mesh)
{
	m_mesh = mesh;
}
