#include "BtPhysicsThread.h"

namespace ma
{
	void PhysicsThread::Update()
	{
		m_pBeginEvent->Wait();

		GetDynamicsWorld()->stepSimulation(GetTimer()->GetFrameDeltaTime());

		m_pEndEvent->Signal();
	}

	PhysicsThread::PhysicsThread()
	{
		m_pBeginEvent = new CMyEvent();
		m_pEndEvent = new CMyEvent();
	}

	PhysicsThread::~PhysicsThread()
	{
        SAFE_DELETE(m_pBeginEvent);
        SAFE_DELETE(m_pEndEvent);
	}

	void PhysicsThread::BeginUpdate()
	{
		m_pBeginEvent->Signal();
	}

	void PhysicsThread::EndUpdate()
	{
		m_pEndEvent->Wait();
	}
}
