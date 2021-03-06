#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "WindowManager.h"
#include "World.h"
using namespace glm;

class CRenderer
{
public:
	CRenderer();
	enum EAttributeEnum
	{
		VERTEX_POS = 0,
		VERTEX_NORMAL = 1,
		TEXTURE_UV = 2
	};
	enum EUniformEnum
	{
		MODEL_MATRIX_4X4 = 3,
		VIEW_MATRIX_4X4 = 4,
		PROJECTION_MATRIX_4X4 = 5,
		OBJECT_COLOR_VEC3 = 6,
		LIGHT_COLOR_VEC3 = 7,
		LIGHT_POS_VEC3 = 8,
		CAMERA_POS_VEC3 = 9
	};

	void Initialize(const Shader& shader, CWindowManager * mgr);
	void SetViewMatrix(const mat4& view);
	void SetProjectionMatrix(const mat4& proj);
	void SetProgram(const Shader& shader);


	void Draw(CEntity * entity);
	void Draw(CWorld * world);
	void Draw(CScene * scene);
	void EnableTexturing(bool flag);
	void ClearBuffer();
	void SwapBuffer();
private:
	//overloads on DrawComponent
	void DrawComponent(CECVisualMesh* mesh);
	bool m_isTexturingEnabled = true;
	//void DrawComponent(Model& model);
	mat4 m_view;// = nullptr;
	mat4 m_projection;// = nullptr;
	Shader m_program;
	CWindowManager * m_windowManager;
};