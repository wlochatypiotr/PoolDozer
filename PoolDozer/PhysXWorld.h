#pragma once
#include "PhysX/PxPhysicsAPI.h"
#include <iostream>

using namespace physx;

class PhysXWorld
{
public:

 	void InitializePhysX();
	void InitializeScene();

	//default callback
	static PxDefaultErrorCallback m_errorCallback;
	//default allocator
	static PxDefaultAllocator m_allocatoraCallback;

	static PxFoundation* m_foundation;
	static PxPhysics* m_physics;
	static PxScene* m_scene;
};