#include "BtPhysicsThread.h"

namespace ma
{
	void PhysicsThread::Run()
	{
		while(true)
		{
			m_pBeginEvent->Wait();

			GetDynamicsWorld()->stepSimulation(GetTimer()->GetFrameDeltaTime());

			m_pEndEvent->Signal();
		}
	}

	PhysicsThread::PhysicsThread()
	{
		m_pBeginEvent = new CMyEvent();
		m_pEndEvent = new CMyEvent();
	}

	PhysicsThread::~PhysicsThread()
	{

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
