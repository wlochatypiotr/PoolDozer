#pragma once
#include "PhysX/PxPhysicsAPI.h"
#include <iostream>

using namespace physx;

class CPhysXWorld
{
public:

	CPhysXWorld();
	~CPhysXWorld();

	void StartUp();
	void ShutDown();


	//default callback
	PxDefaultErrorCallback m_errorCallback;
	//default allocator
	PxDefaultAllocator m_allocatorCallback;

	PxFoundation* m_foundation = nullptr;
	PxPhysics* m_physics = nullptr;
	PxScene* m_scene = nullptr;
};