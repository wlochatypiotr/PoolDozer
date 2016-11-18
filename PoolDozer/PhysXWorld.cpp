#include "PhysXWorld.h"
#include <afx.h>



PhysXWorld::PhysXWorld()
{
	//do nothing
}

PhysXWorld::~PhysXWorld()
{	//do nothing
}

void PhysXWorld::StartUp()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocatorCallback, m_errorCallback);
	ASSERT(m_foundation);
	if (m_foundation != nullptr)
		std::cout << "PhysX foundation successfully created." << std::endl;
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale());
	ASSERT(m_physics);
	if (m_physics != nullptr)
		std::cout << "PhysX Physics successfully created." << std::endl;

	//scene
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	m_scene = m_physics->createScene(sceneDesc);
	ASSERT(m_scene);
	if (m_scene != nullptr)
		std::cout << "PhysX Scene successfully created." << std::endl;
}

void PhysXWorld::ShutDown()
{
	m_scene->release();
	m_physics->release();
	m_foundation->release();
}

