#pragma once

//Board

namespace models
{

	GLfloat BoardVertex[] = { -1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,

	-1.0f, -1.0f, -0.1f,
	-1.0f, 1.0f, -0.1f,
	1.0f, 1.0f, -0.1f,
	1.0f, -1.0f, -0.1f,

	-0.95f, -0.95f, -0.1f,
	-0.95f, 0.95f, -0.1f,
	0.95f, 0.95f, -0.1f,
	0.95f, -0.95f, -0.1f,

	-0.95f, -0.95f, -0.05f,
	-0.95f, 0.95f, -0.05f,
	0.95f, 0.95f, -0.05f,
	0.95f, -0.95f, -0.05f };

	GLuint BoardIndex[] = 
	{ 0, 1, 3,
	1, 2, 3,
	1, 0, 4,
	4, 5, 1,
	1, 5, 2,
	5, 6, 2,
	2, 6, 3,
	6, 7, 3,
	3, 7, 0,
	7, 4, 0,
	4, 5, 8,
	8, 4, 9,
	5, 6, 9,
	9, 6, 10,
	10, 6, 7,
	10, 7, 11,
	11, 7, 4,
	11, 4, 8,
	8, 9, 12,
	12, 9, 13,
	9, 10, 13,
	13, 10, 14,
	10, 11, 15,
	10, 15, 14,
	11, 8, 12,
	11, 12, 15,
	12, 13, 15,
	15, 13, 14 };

	GLfloat DiceVertex[] = {
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5, 0.5f, -0.5f,
	0.5f,-0.5f, -0.5f,

	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5, 0.5f, 0.5f,
	0.5f,-0.5f, 0.5f
	};

	GLuint DiceIndex[] = {
		0,1,2,
		0,2,3,
		4,5,0,
		0,5,1,
		4,0,7,
		0,3,7,
		3,6,7,
		3,2,6,
		5,6,1,
		1,6,2,
		4,5,7,
		5,6,7


	};

}