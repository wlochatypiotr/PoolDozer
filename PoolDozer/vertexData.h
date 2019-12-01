#pragma once
#include <vector>

struct vertexData
{
	//vertex position
	float x;
	float y;
	float z;

	//texture mapping
	float tx;
	float ty;

	//Normals
	float nx;
	float ny;
	float nz;
	float nw;
};

struct uintx3
{
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

struct floatx3
{
	float x;
	float y;
	float z;
};

struct floatx2
{
	float x;
	float y;
};

struct Triangle
{
	std::vector<unsigned int> mIndices;
	std::string mMaterialName;
	unsigned int mMaterialIndex;

	bool operator<(const Triangle& rhs)
	{
		return mMaterialIndex < rhs.mMaterialIndex;
	}
};

struct PNTIWVertex
{
	floatx3 mPosition;
	floatx3 mNormal;
	floatx2 mUV;

	bool operator==(const PNTIWVertex& rhs) const
	{

		bool result1 = mPosition.x == rhs.mPosition.x && mPosition.y == rhs.mPosition.y && mPosition.z == rhs.mPosition.z;
		bool result2 = mNormal.x == rhs.mNormal.x && mNormal.y == rhs.mNormal.y && mNormal.z == rhs.mNormal.z;
		bool result3 = mUV.x == rhs.mUV.x && mUV.y == rhs.mUV.y;

		return result1 && result2 && result3;
	}
};
