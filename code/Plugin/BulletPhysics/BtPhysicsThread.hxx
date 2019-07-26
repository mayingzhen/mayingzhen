#include "BtPhysicsThread.h"

namespace ma
{
	void PhysicsThread::ThreadLoop()
	{
		//while(!m_bExit)
		{
			//m_pBeginEvent->Wait();

			m_bEnd = false;

			if ( GetPhysicsSystem()->GetDynamicsWorld() )
				GetPhysicsSystem()->GetDynamicsWorld()->stepSimulation(GetTimer()->GetFrameDeltaTime());

			m_bEnd = true;
		}
	}

	PhysicsThread::PhysicsThread()
	{
		//m_pBeginEvent = new CMyEvent();
	}

	PhysicsThread::~PhysicsThread()
	{
        //SAFE_DELETE(m_pBeginEvent);
	}

	void PhysicsThread::Stop()
	{
		//m_bExit = true;
		//m_pBeginEvent->Signal();

		//Thread::Stop();
	}

	void PhysicsThread::BeginUpdate()
	{
		//m_pBeginEvent->Signal();
	}

	void PhysicsThread::EndUpdate()
	{
		while(!m_bEnd)
		{
			std::this_thread::yield();
		}
	}
}
