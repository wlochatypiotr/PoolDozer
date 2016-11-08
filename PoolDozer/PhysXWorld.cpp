#include "PhysXWorld.h"

PxDefaultErrorCallback PhysXWorld::m_errorCallback;
PxDefaultAllocator PhysXWorld::m_allocatoraCallback;

PxFoundation* PhysXWorld::m_foundation = nullptr;
PxPhysics* PhysXWorld::m_physics = nullptr;
PxScene* PhysXWorld::m_scene = nullptr;



void PhysXWorld::InitializePhysX()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocatoraCallback, m_errorCallback);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale());

}

void PhysXWorld::InitializeScene()
{
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	m_scene = m_physics->createScene(sceneDesc);
}
